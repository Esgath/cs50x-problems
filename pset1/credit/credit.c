#include <cs50.h>
#include <stdio.h>

bool is_valid(long credit_card);
void find_card_company(long credit_card);

int main(void)
{
    long credit_card = get_long("Number: ");
    
    // check if number is a valid card number
    bool is_valid_card = is_valid(credit_card);
    if (is_valid_card)
    {
        find_card_company(credit_card);
    }
    else
    {
        printf("INVALID\n");
    }
}

bool is_valid(long credit_card)
{
    int multiplied_sum = 0;
    int rest_sum = 0;
    int parity = 1;
    while (credit_card > 0) 
    {
        int digit = credit_card % 10;
        // multiply even numbers
        if (parity % 2 == 0)
        {
            digit = digit * 2;
            while (digit > 0)
            {
                int lower_digit = digit % 10;
                multiplied_sum += lower_digit;
                // get rid of last digit
                digit /= 10;
            }
        } 
        else 
        {
            rest_sum += digit;
        }
        // get rid of last digit
        credit_card /= 10;
        // parity of next number
        parity++;
    }
    int sum_of_both = multiplied_sum + rest_sum;
    if (sum_of_both % 10 == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void find_card_company(long credit_card)
{   
    long credit_card_copy = credit_card;
    
    // find length first digit and last digit
    int first_digit;
    int last_digit = credit_card % 10; 
    int length = 0;
    while (credit_card >= 10 || credit_card != 0)
    {
        first_digit = credit_card;
        length++;
        credit_card /= 10;
    }
    
    // find second digit
    int second_digit;
    for (int i = 0; i < length-2; i++)
    {
        credit_card_copy /= 10;
        second_digit = credit_card_copy % 10;
    }

    // decide which company/INVALID
    if (length == 15 && first_digit == 3 && (second_digit == 4 || second_digit == 7))
    {
        printf("AMEX\n");
    }
    else if (length == 16 || length == 13)
    {
        if (first_digit == 5 && (second_digit == 1 || second_digit == 2 || second_digit == 3 || second_digit == 4 || second_digit == 5))
        {
            printf("MASTERCARD\n");    
        }
        else if (first_digit == 4)
        {
            printf("VISA\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }
}