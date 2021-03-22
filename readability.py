from cs50 import get_string


def CalculateReadability(userInput):
    inputLength = len(userInput)
    userInput = userInput.upper()
    letterCount = 0
    # Initialised at 1 to account for the final word
    wordCount = 1
    sentenceCount = 0
    
    for i in range(0, inputLength, 1):
        currentCharIndex = ord(userInput[i])
        
        # Checks to see if the char is within the ASCII bounds of A - Z
        if currentCharIndex >= 65 and currentCharIndex <= 90:
            letterCount += 1
        # Checks for a space
        if currentCharIndex == 32:
            wordCount += 1
        # Checks for ! . ?
        if currentCharIndex == 33 or currentCharIndex == 46 or currentCharIndex == 63:
            sentenceCount += 1
    
    wordsPer100 = wordCount / 100
    # Number of letters per 100 words
    lettersPer100 = letterCount / wordsPer100
    # Number of sentences per 100 words
    sentencesPer100 = sentenceCount / wordsPer100
    
    # Coleman-Liau index
    return (0.0588 * lettersPer100) - (0.296 * sentencesPer100) - 15.8


def main():
    userInput = get_string("Text: ")
    unroundedScore = CalculateReadability(userInput)
    roundedScore = round(unroundedScore)
    if roundedScore < 1:
        print("Before Grade 1")
    elif roundedScore >= 1 and roundedScore < 16:
        roundedScoreString = str(roundedScore)
        print("Grade " + roundedScoreString)
    else:
        print("Grade 16+")
        
        
if __name__ == "__main__":
    main()