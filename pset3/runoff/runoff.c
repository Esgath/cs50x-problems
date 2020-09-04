#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max voters and candidates
#define MAX_VOTERS 100
#define MAX_CANDIDATES 9

// preferences[i][j] is jth preference for voter i
int preferences[MAX_VOTERS][MAX_CANDIDATES];

// Candidates have name, vote count, eliminated status
typedef struct
{
    string name;
    int votes;
    bool eliminated;
}
candidate;

// Array of candidates
candidate candidates[MAX_CANDIDATES];

// Numbers of voters and candidates
int voter_count;
int candidate_count;

// Function prototypes
bool vote(int voter, int rank, string name);
void tabulate(void);
bool print_winner(void);
int find_min(void);
bool is_tie(int min);
void eliminate(int min);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: runoff [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX_CANDIDATES)
    {
        printf("Maximum number of candidates is %i\n", MAX_CANDIDATES);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
        candidates[i].eliminated = false;
    }

    voter_count = get_int("Number of voters: ");
    if (voter_count > MAX_VOTERS)
    {
        printf("Maximum number of voters is %i\n", MAX_VOTERS);
        return 3;
    }

    // Keep querying for votes
    for (int i = 0; i < voter_count; i++)
    {

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            // Record vote, unless it's invalid
            if (!vote(i, j, name))
            {
                printf("Invalid vote.\n");
                return 4;
            }
        }

        printf("\n");
    }

    // Keep holding runoffs until winner exists
    while (true)
    {
        // Calculate votes given remaining candidates
        tabulate();
        // for (int i = 0; i < candidate_count; i++)
        // {
        //     int candidate_votes = candidates[i].votes;
        //     printf("%i - %i votes\n", i, candidate_votes);
        //     printf("--------------------------------------------\n\n");
        // }

        // Check if election has been won
        bool won = print_winner();
        if (won)
        {
            break;
        }

        // Eliminate last-place candidates
        int min = find_min();
        bool tie = is_tie(min);

        // If tie, everyone wins
        if (tie)
        {
            for (int i = 0; i < candidate_count; i++)
            {
                if (!candidates[i].eliminated)
                {
                    printf("%s\n", candidates[i].name);
                }
            }
            break;
        }

        // Eliminate anyone with minimum number of votes
        eliminate(min);

        // Reset vote counts back to zero
        for (int i = 0; i < candidate_count; i++)
        {
            candidates[i].votes = 0;
        }
    }
    return 0;
}

// Record preference if vote is valid
bool vote(int voter, int rank, string name)
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i].name) == 0)
        {
            preferences[voter][rank] = i;
            return true;
        }
    }
    return false;
}

int find_candidate_id(int voter_index, int current_rank)
{
    if (current_rank == candidate_count - 1)
    {
        return preferences[voter_index][current_rank];
    }
    else
    {
        int candidate_id = preferences[voter_index][current_rank];
        if (candidates[candidate_id].eliminated)
        {
            return find_candidate_id(voter_index, current_rank + 1);
        }
        else
        {
            return candidate_id;
        }
    }
}

// Tabulate votes for non-eliminated candidates
void tabulate(void)
{

    // Iterate over voters
    for (int j = 0; j < voter_count; j++)
    {   
        int candidate_id = find_candidate_id(j, 0);
        candidates[candidate_id].votes++;
    }
    // for (int n = 0; n < candidate_count; n++)
    // {
    //     printf("Candidate %i:\n >>> %i votes\n", n, candidates[n].votes);
    // }
}

// Print the winner of the election, if there is one
bool print_winner(void)
{
    int votes_border = voter_count / 2;
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes > votes_border)
        {
            printf("%s\n", candidates[i].name);
            return true;
        }
    }
    return false;
}

// Return the minimum number of votes any remaining candidate has
int find_min(void)
{
    int next_smallest = voter_count;
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes < next_smallest && !candidates[i].eliminated)
        {
            next_smallest = candidates[i].votes;
        }
    }

    return next_smallest;
}

// Return true if the election is tied between all candidates, false otherwise
bool is_tie(int min)
{
    // Find number of candidates who are not eliminated
    int running_candidates = 0;
    for (int j = 0; j < candidate_count; j++)
    {
        if (!candidates[j].eliminated)
        {
            running_candidates++;
        }
    }

    // Count number of candidates who have vote count same as 'min'
    int n_of_same_scores = 0;
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes == min)
        {
            n_of_same_scores++;
        }
    }

    // If number of candidates with score as 'min' equal to all candidates left then return true -> tie
    if (n_of_same_scores == running_candidates)
    {
        return true;
    }
    else
    {
        return false;
    }   
}

// Eliminate the candidate (or candidiates) in last place
void eliminate(int min)
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes == min && !candidates[i].eliminated)
        {
            candidates[i].eliminated = true;
        }
    }
}
