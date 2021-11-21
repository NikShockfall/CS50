import csv
from sys import argv, exit


dna = []


def main():
    # Check if # of command-line argument is correct
    if len(argv) != 3:
        print("Usage: python dna.py DATABASE SEQUENCE")
        exit(1)

    # Read csv file
    datafile = argv[1]
    with open(datafile, "r") as file:
        rdata = csv.DictReader(file)
        for row in rdata:
            dna.append(row)
            # print(row)

    header = list(dna[0:1][0])

    # Read sequence file
    seqfile = argv[2]
    with open(seqfile, "r") as file:
        for line in file:
            print(finder(header, line))

# Function to determine match
def finder(header, sequence):
    for data in dna:
        check = True    
        for i in range(1, len(header)):
            strand = header[i]
            count = int(data[strand])
            if check:
                check = strand*count in sequence and strand*(count+1) not in sequence

        if check:
            return data["name"]

    return "No match"


if __name__ == "__main__":
    main()