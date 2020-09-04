#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i; j < candidate_count; j++)
        {
            if (i == j)
            {
                preferences[ranks[i]][ranks[j]] = 0;
                // printf("PREFERENCE ZERO[%i][%i] = %i\n", ranks[i], ranks[j], preferences[ranks[i]][ranks[j]]);
            }
            else if (!preferences[ranks[i]][ranks[j]])
            {
                preferences[ranks[i]][ranks[j]] = 1;
                // printf("PREFERENCE NEW[%i][%i] = %i\n", ranks[i], ranks[j], preferences[ranks[i]][ranks[j]]);
            }
            else
            {
                preferences[ranks[i]][ranks[j]]++;
                // printf("PREFERENCE ADD ONE[%i][%i] = %i\n", ranks[i], ranks[j], preferences[ranks[i]][ranks[j]]);
            }
        }
    }
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    pair_count = 0;
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            // Check which one is prefered
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;            
                // printf("PAIR(%i vs %i) Winner: %i Loser: %i\n", i, j, pairs[pair_count].winner, pairs[pair_count].loser);
                // printf("PAIR COUNT: %i\n", pair_count);          
                pair_count++;
            }
        }
    }
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // DEBUG Print original:
    // printf("BEFORE SORTING:\n PAIR COUNT%i\n", pair_count);
    // for (int j = 0; j < pair_count; j++)
    // {
    //     printf("%i. Winner: %i, Loser: %i\n", j+1, pairs[j].winner, pairs[j].loser);
    //     printf(">>> Winner votes=%i, Loser votes=%i\n\n", preferences[pairs[j].winner][pairs[j].loser], preferences[pairs[j].loser][pairs[j].winner]);

    // }

    int remember = 0;
    int swap_counter = -1;

    while (swap_counter != 0)
    {
        swap_counter = 0;
        for (int i = pair_count-1; i > remember; i--)
        {
            int winner = pairs[i].winner;
            int loser = pairs[i].loser;
            int winner_votes = preferences[winner][loser];
            int go_left_winner = pairs[i-1].winner;
            int go_left_loser = pairs[i-1].loser;
            int go_left_winner_votes = preferences[go_left_winner][go_left_loser];
            // Compare strength of victory;
            if (winner_votes > go_left_winner_votes)
            {
                // Swap values
                pair temp = pairs[i-1];
                pairs[i-1] = pairs[i];
                pairs[i] = temp;
                swap_counter++;
            }
        }
        remember++;
    }
    // DEBUG Print sorted:
    // printf("AFTER SORTING:\n PAIR COUNT%i\n", pair_count);
    // for (int j = 0; j < pair_count; j++)
    // {
    //     printf("%i. Winner: %i, Loser: %i\n", j+1, pairs[j].winner, pairs[j].loser);
    //     printf(">>> Winner votes=%i, Loser votes=%i\n\n", preferences[pairs[j].winner][pairs[j].loser], preferences[pairs[j].loser][pairs[j].winner]);

    // }
}

bool check_edges_help(bool visited[], int index)
{
    if (visited[index])
    {
        return true;
    }
    visited[index] = true;
    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[index][i])
        {
           if (check_edges_help(visited, i))
           {
               return true;
           }
        }   
    }
    return false;
}
    
bool check_edges(int index)
{   
    bool vertices_visited[candidate_count];
    for (int i = 0; i < candidate_count; i++)
    {
        vertices_visited[i] = false;
    }
    return check_edges_help(vertices_visited, index);
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        locked[pairs[i].winner][pairs[i].loser] = true;
        // Look for cycle
        if (check_edges(pairs[i].winner))
        {
            locked[pairs[i].winner][pairs[i].loser] = false;
        }
    }
}

// Print the winner of the election
void print_winner(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        int count = 0;
        for (int j = 0; j < candidate_count; j++)
        {
            if (!locked[j][i])
            {
                count++;
            }

        }
        if (count == candidate_count)
        {
            printf("%s\n", candidates[i]);
        }
    }
}

