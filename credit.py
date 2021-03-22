from cs50 import get_int

#Used to test the length and starting digits by division
ThirteenDigitTest =      100000000000
FourteenDigitTest =     1000000000000
FithteenDigitTest =    10000000000000
SixteenDigitTest =    100000000000000
SeventeenDigitTest = 1000000000000000

firstDigitTest = 0
cardAsString = " "
cardNumberLength = 0

def CheckLength(creditCardNumber):
    #Check length of number by division
    global cardNumberLength 
    if creditCardNumber / SixteenDigitTest >= 10 and creditCardNumber / SeventeenDigitTest < 100:
        cardNumberLength = 16
        return True
    elif creditCardNumber / FithteenDigitTest >= 10:
        cardNumberLength = 15
        return True
    
    elif creditCardNumber / ThirteenDigitTest >= 10 and creditCardNumber / ThirteenDigitTest < 100:
        cardNumberLength = 13
        return True

    else:    #if INVALID
        cardNumberLength = 0
        UserFailedTest()
        return False

def CheckStartingCharacters(creditCardNumber):
    global firstDigitTest 
    #Places a decimal point after the second character in card number to query first characters
    if cardNumberLength == 13:
        firstDigitTest = creditCardNumber / ThirteenDigitTest
        if CheckFirstTwoAmex():
            return True
        elif CheckFirstTwoVisa():
            return True
    elif cardNumberLength == 15:
        firstDigitTest = creditCardNumber / FithteenDigitTest
        if CheckFirstTwoAmex():
            return True
    elif cardNumberLength == 16:
        firstDigitTest = creditCardNumber / SixteenDigitTest
        if CheckFirstTwoVisa():
            return True
        elif CheckFirstTwoMasterCard():
            return True
    else:
        #If switch doesn't return true, user has failed
        UserFailedTest()
        return False

def CheckFirstTwoAmex():

    if firstDigitTest >= 34 and firstDigitTest < 35:
        return True
    elif firstDigitTest >= 37 and firstDigitTest < 38:
        return True
    else:
        return False

def CheckFirstTwoVisa():

    if firstDigitTest >= 40 and firstDigitTest < 50:
        return True
    else:
        return False

def CheckFirstTwoMasterCard():
    if firstDigitTest >= 51 and firstDigitTest < 56:
        return True
    else:
        return False

def CheckLuhnsTest(creditCardNumber):
    sumOfDigits = 0

    global cardAsString 
    cardAsString = str(creditCardNumber)

    #Applies Luhns Algorithm (add and multiply digits) dependant on card number lenght
    if cardNumberLength == 16:
        sumOfDigits += AddAlternateDigits(0, True)
        sumOfDigits += AddAlternateDigits(1, False)
    else:
        sumOfDigits += AddAlternateDigits(1, True)
        sumOfDigits += AddAlternateDigits(0, False)
        
    if sumOfDigits % 10 == 0:
        return True
    else:
        UserFailedTest()
        return False

#Used to add every other digit in credit card number and double if required
def AddAlternateDigits(startingDigit, doubleAnswers):
    global cardNumberLength
    global cardAsString 
    runningTotal = 0
    for i in range(startingDigit, cardNumberLength, 2):
        #Converts relevant char of card number to an int
        x = int(cardAsString[i])
       
        if doubleAnswers:
            answerDoubled = x * 2
            #If the doubled answer has more than two digits, adds those individual digits. 
            if x * 2 > 9:
                runningTotal += answerDoubled - 9
            else:
                runningTotal += answerDoubled
        else:
            runningTotal += x

    return runningTotal

def DisplayCardType():
    if firstDigitTest < 40:
        print("AMEX")
    elif firstDigitTest >= 50:
        print("MASTERCARD")
    else:
        print("VISA")
        
def UserFailedTest():
    print("INVALID")
    
def main():

    creditCardNumber = get_int("Card number: ")
        
    if CheckLength(creditCardNumber):
        if CheckStartingCharacters(creditCardNumber):
            if CheckLuhnsTest(creditCardNumber):
                DisplayCardType()
                
if __name__ == "__main__":
    main()