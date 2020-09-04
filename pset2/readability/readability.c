#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int count_type(string user_text, string choose_type);
int coleman_index(int n_letters, int n_words, int n_sentences);

int main(void)
{
    // Ask user for input
    string user_text = get_string("Text: ");
    // Count number of letters
    int number_of_letters = count_type(user_text, "letters");
    int number_of_words = count_type(user_text, "words");
    int number_of_sentences = count_type(user_text, "sentences");
    int grade = coleman_index(number_of_letters, number_of_words, number_of_sentences);

    if (grade >= 16)
    {
        printf("Grade 16+\n");
    }
    else if (grade < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %i\n", grade);
    }

}

int coleman_index(int n_letters, int n_words, int n_sentences)
{
    // index = 0.0588 * L - 0.296 * S - 15.8
    float index_value;
    float avg_n_letters = (100 * n_letters) / (float) n_words;
    float avg_n_sentences = (100 * n_sentences) / (float) n_words;
    index_value = (0.0588 * avg_n_letters) - (0.296 * avg_n_sentences) - 15.8;
    return (int) roundf(index_value);
}

int count_type(string user_text, string choose_type)
{
    // Count letters/words
    int count = 0;
    
    // When counting words will store number of letters after punctuation sign
    int letter_count = 0;
    
    // Get length of user input
    int length = strlen(user_text);

    string punctuations = ".,?!' ";
    int punctuations_length = strlen(punctuations);
    bool is_punctuation;

    for (int i = 0; i < length; i++)
    {
        // Check if char is a punctuation and set bool variable is_punctuation accordingly   
        for (int j = 0; j < punctuations_length; j++)
        {
            is_punctuation = (user_text[i] == punctuations[j]);
            if (is_punctuation)
            {
                break;
            }         
        }

        // Decide if is_punctuation and add 1 to count according to type -> letters/words
        if (!is_punctuation)
        {   
            // If choose_type is equal to "letters" then count letters
            if (strcmp(choose_type, "letters") == 0)
            {
                if (isalpha(user_text[i]))
                {
                    count++;
                }
                
            }
            else
            {   
                letter_count++;
            }
            
        }
        else
        {   
            // If choose_type is equal to "words" then count words
            if (strcmp(choose_type, "words") == 0)
            {
                // Make sure to count words and not punctuation signs for example don't count "Argh!!!" as 3 but 1
                if (letter_count == 0)
                {
                    ;
                }
                else
                {
                    // Count for example "You're" as one
                    if (user_text[i] == '\'')
                    {
                        ;
                    }
                    else
                    {
                        count++;
                        // Reset letter_count
                        letter_count = 0;
                    }
                    
                }
            }
            // If choose_type is equal to "stentences" then count sentences
            else if (strcmp(choose_type, "sentences") == 0)
            {
                if ((user_text[i] == '.' || user_text[i] == '!' || user_text[i] == '?') && letter_count != 0)
                {
                    count++;
                    // Reset letter_count
                    letter_count = 0;
                }
            }
        }
        
    }
    return count;
}

// Should have written in 3 different functions :(