from cs50 import get_string
import re


def main():
    """Compute the approximate grade level needed to comprehend some text."""
    
    # Get user input
    text = get_string("Text: ")
    letters = 0
    words = 0
    sentences = 0
    
    end_of_sentence = [".", "!", "?"]
    # Count number of words, sentences and letters
    for i in range(len(text)):
        if text[i] in end_of_sentence:
            sentences += 1
            words += 1
        elif text[i] == "\n":
            pass
        elif text[i] == " ":
            if text[i-1] in end_of_sentence:
                pass
            else:
                words += 1
        else:
            if text[i] in [",", "'", "-" , "+", "\"", ";", ":"]:
                pass
            else:
                letters += 1

    # Count average number of letters and sentences per 100 words
    avg_letters = (letters * 100) / words
    avg_sentences = (sentences * 100) / words

    coleman_index = coleman(avg_letters, avg_sentences)
    
    # Print Grade
    if coleman_index < 1:
        print("Before Grade 1")
    elif coleman_index >= 16:
        print("Grade 16+")
    else:
        print(f"Grade {coleman_index}")


def coleman(avg_letters, avg_sentences):
    """Use Coleman-Liau formula."""
    result = round(0.0588 * avg_letters - 0.296 * avg_sentences - 15.8)
    return result


main()