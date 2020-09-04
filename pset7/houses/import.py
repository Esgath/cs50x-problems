from sys import exit, argv
import csv
import cs50
import re


def main():
    """Import data from csv file to database"""
    
    # Check for correct number of CL arguments
    if (len(argv) != 2):
        print("USAGE: python3 import.py csv_file")
        exit(1)

    # Open csv file and store results in list
    students = []
    with open(argv[1], "r") as file:
        reader = csv.DictReader(file)
        for row in reader:
            students.append(row)

    # Insert into database
    db = cs50.SQL("sqlite:///students.db")
    for student in students:
        first, middle, last = find_name(student)
        house = student['house']
        birth = student['birth']
        db.execute("INSERT INTO students (first, middle, last, house, birth)\
             VALUES(?, ?, ?, ?, ?)", first, middle, last, house, birth)
    exit(0)


def find_name(student):
    """Returns first, middel and last name of sutdent"""
    
    # Find all names
    a = re.findall(r"[a-zA-Z-]+", student['name'])
    null_val = None
    if len(a) == 0:
        return null_val, null_val, null_val
    elif len(a) == 1:
        return a[0], null_val, null_val
    elif len(a) == 2:
        return a[0], null_val, a[1]
    elif len(a) == 3:
        return a[0], a[1], a[2]


main()
