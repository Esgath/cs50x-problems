from sys import exit, argv 
import cs50


def main():
    """"""

    # Check if correct number CL arguments
    if len(argv) != 2:
        print("ERROR!\nUSAGE: python3 roster.py house_name")
        exit(1)

    house = argv[1].capitalize()
    
    # Select from database
    db = cs50.SQL("sqlite:///students.db")
    people = db.execute("SELECT first, middle, last, birth FROM students\
         WHERE house = ? ORDER BY last, first", house)
    
    # Print output
    for person in people:

        # List comprehension
        names_to_print = [person[key] for key in person.keys()\
                          if key != 'birth' and person[key] != None]

        # Create names string
        names_string = " ".join(names_to_print)
        print(names_string, end=',')
        print(" born", person['birth'])


main()