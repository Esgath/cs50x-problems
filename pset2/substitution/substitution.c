#include <stdio.h>
#include <cs50.h>
#include <ctype.h>
#include <string.h>

string encipher_fun(string plaintext, string cipher_key);

int main(int argc, string argv[])
{   
    string cipher_key = argv[1];
    
    // Check number of command line arguments
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    
    // Check key's length
    int key_length = strlen(cipher_key);
    if (key_length != 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }    
    
    // Look for wrong characters
    for (int i = 0; i < key_length; i++)
    {
        if (!isalpha(cipher_key[i]))
        {
            printf("One of chars is not an alpha char.\n");
            return 1;
        }
    }
    
    // Check if characters repeat
    int count;
    for (int i = 0; i < key_length; i++)
    {
        count = 0;
        // Turn to lowercase
        char char_low = tolower(cipher_key[i]);
        for (int j = 0; j < key_length; j++)
        {
            if (char_low == tolower(cipher_key[j]))
            {
                count++;
            }
            if (count > 1)
            {
                printf("Key must not contain repeated characters.\n");
                return 1;
            }
        }
    }
    
    // Ask for plaintext
    string plaintext = get_string("plaintext: ");
    string output = encipher_fun(plaintext, cipher_key);
    
    // Print ciphertext and exit program
    printf("ciphertext: %s\n", output);
    return 0;
}

string encipher_fun(string plaintext, string cipher_key)
{
    int capital_a_number = 65;
    int text_length = strlen(plaintext);
    string output = plaintext;
    
    for (int i = 0; i < text_length; i++)
    {
        if (isalpha(plaintext[i]))
        {
            // Find position
            int current_char_number = toupper(plaintext[i]);
            int position = current_char_number - capital_a_number;
            
            // Use position to find char in cipher_key
            char cipher_char = cipher_key[position];
            
            if (islower(plaintext[i]))
            {
                char lower_char = tolower(cipher_char);
                output[i] = lower_char;
            }
            else
            {
                char upper_char = toupper(cipher_char);
                output[i] = upper_char;
            } 
        }
        else
        {
            output[i] = plaintext[i];
        } 
    }
    return output;
}