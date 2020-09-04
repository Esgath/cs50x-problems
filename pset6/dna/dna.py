from sys import exit, argv
import csv, re


def main():
    """Identifies to whom a sequence of DNA belongs."""
    
    # Check if correct number of command line arguments
    if len(argv) != 3:
        print("Usage: python3 dna.py data.csv sequence.txt")
        exit(1)

    # Store str counts in list of dictionaries and store str patterns in a list
    str_counts, str_patterns = load_str(argv[1])
    
    # Store DNA sequence
    dna_sequence = load_dna(argv[2]).rstrip("\n")

    # Create dict to store highest run for each STR
    counts_found = {}

    # For every pattern find it inside dna_sequence using regex
    for pattern in str_patterns:
        # Rength of pattern
        length = len(pattern)
        
        # Findall returns list of all found runs
        found = re.findall(f"(?:{pattern})+", dna_sequence)
        
        # Variable used to find highest run
        highest = 0
        
        # Find highest run
        for element in found:
            # divide length of list by the length of pattern and check if highest
            count = len(element) / length
            if count > highest:
                highest = count
            else:
                pass
        
        counts_found[pattern] = highest
    
    person = find_person(str_counts, counts_found)
    print(person)
   

def find_person(str_counts, counts_found):
    """Finds person based on STR counts found in DNA sequence"""
    for element in str_counts:
        count = 0
        for pattern, value in counts_found.items():
            if value == int(element[pattern]):
                count += 1
            if count == len(counts_found):
                return element['name']
    return 'No match'


def load_str(file):
    """Opens file with str counts and names and returns list of dictionaries.\
    """

    str_counts = []
    str_patterns = []
    
    with open(file, "r") as file_str:
        reader = csv.DictReader(file_str)
        for row in reader:
            str_counts.append(row)

    for element in str_counts[0]:
        if element == 'name':
            pass
        else:
            str_patterns.append(element)

    return str_counts, str_patterns


def load_dna(file):
    """
    Opens file with dna sequence and returns string of \
    nucleotides bases sequence.
    """

    with open(file, "r") as file_dna:
        reader = file_dna.read()
    return reader   


main()