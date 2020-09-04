from cs50 import get_string


def main():
    """
    Determines whether a provided credit card number is valid
    accordint to Luhn's algorithm.
    """

    # Asku user for credit card number and store it
    card_number = get_string("Number: ")

    # How credit card's start:
    AMERICAN_EXPRESS = ["34", "37"]
    MASTER_CARD = ["51", "52", "53", "54", "55"]
    VISA = ["4"]

    valid = luhn(card_number)

    if valid:
        # Check for type of card VISA/MASTER_CARD/AMERICAN_EXPRESS
        if card_number[:2] in AMERICAN_EXPRESS:
            print("AMEX")
        elif card_number[:2] in MASTER_CARD:
            print("MASTERCARD")
        elif card_number[:1] in VISA:
            print("VISA")
    else:
        print("INVALID")


def luhn(number):
    """
    Luhn's Algorithm
    """

    # Get every other digit and store it in string
    every_other = number[-2::-2]
    rest_of_digits = number[::-2]

    # Multiply elemnts in every_other by 2 and store them in list
    every_other_list = []
    for i in every_other:
        i = int(i) * 2
        every_other_list.append(str(i))
    
    # Sum DIGITS of every element in every_other_list
    sum_every_other = 0
    for element in every_other_list:
        for digit in element:
            sum_every_other += int(digit)

    # Add all digits inside rest_of_digits    
    sum_rest_of_digits = 0
    for j in rest_of_digits:
        sum_rest_of_digits += int(j)

    # Add sum_every_other to the sum_rest_of_digits and convert to string
    summed = str(sum_every_other + sum_rest_of_digits)

    # If last digit is 0 then card is valid
    if summed[-1] == "0":
        return True
    else:
        return False


main()