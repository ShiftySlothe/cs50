from csv import reader, DictReader
from sys import argv, exit


def main():
    
    # Check for CLA
    if len(argv) != 3:
        print("Usage: python dna.py data.csv sequence.txt")
        exit()
        
    database = argv[1]
    sequences = argv[2]
    
    # Get dict from CSV
    with open(database) as csvfile:
        reader = DictReader(csvfile)
        dictList = list(reader)
    # Get list from sequences
    with open(sequences) as file:
        sequence = file.read()
        
    # Get most consecutive repeats
    maxCounts = []
    
    for i in range(1, len(reader.fieldnames)):
        dnaValue = reader.fieldnames[i]
        maxCounts.append(0)
        # Find dnaValue in dna sequence
        for j in range(len(sequence)):
            STRCount = 0
            # If match found, start counting repeats
            if sequence[j:(j + len(dnaValue))] == dnaValue:
                k = 0
                while sequence[(j + k):(j + k + len(dnaValue))] == dnaValue:
                    STRCount += 1
                    k += len(dnaValue)
                # If greater than previous max, update
                if STRCount > maxCounts[i - 1]:
                    maxCounts[i - 1] = STRCount
                
    # Check database
    for i in range(len(dictList)):
        matches = 0
        for j in range(1, len(reader.fieldnames)):
            if int(maxCounts[j - 1]) == int(dictList[i]  [reader.fieldnames[j]]):
                matches += 1
            if matches == (len(reader.fieldnames) - 1):
                print(dictList[i]['name'])
                exit(0)
                
    print("No match")

if __name__ == "__main__":
    main()