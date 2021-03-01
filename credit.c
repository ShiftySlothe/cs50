#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>

//Used to test the length and starting digits by division
const long ThirteenDigitTest =     1000000000000;
const long FourteenDigitTest =     10000000000000;
const long FithteenDigitTest =    100000000000000;
const long SixteenDigitTest =    1000000000000000;
const long SeventeenDigitTest = 10000000000000000;
long firstDigitTest = 0;

long creditCardNumber = 0;
char cardAsString[16];
int cardNumberLength = 0;

bool isCreditCard = false;

//Method References
bool CheckLength();
bool CheckStartingCharacters();
bool CheckFirstTwoAmex();
bool CheckFirstTwoVisa();
bool CheckFirstTwoMasterCard();
bool CheckLuhnsTest();
int AddAlternateDigits(int startingDigit, bool doubleAnswers);
void DisplayCardType();
void UserFailedTest();

int main (void)
{
   creditCardNumber = get_long("Enter your credit card number, no hyphens: ");

   isCreditCard = CheckLength();

   if(isCreditCard)
   {
      isCreditCard = CheckStartingCharacters();
   }
   if(isCreditCard)
   {
      isCreditCard = CheckLuhnsTest();
   }
   if(isCreditCard)
   {
      DisplayCardType();
   }
}

bool CheckLength()
{
   //Check length of number by division

   if (creditCardNumber / SixteenDigitTest >= 1 && creditCardNumber / SeventeenDigitTest < 10)
   {
      cardNumberLength = 16;
      return true;
   }
   else if (creditCardNumber / FithteenDigitTest >= 1)
   {
      cardNumberLength = 15;
      return true;
   }
   else if (creditCardNumber / ThirteenDigitTest >= 1 && creditCardNumber / ThirteenDigitTest < 10)
   {
      cardNumberLength = 13;
      return true;
   }

   else    //if INVALID
   {
      cardNumberLength = 0;
      UserFailedTest();
      return false;
   }
}

bool CheckStartingCharacters()
{
   //Places a decimal point after the first character in card number to query first characters
   switch(cardNumberLength)
   {
      case 13:
         firstDigitTest = creditCardNumber / ThirteenDigitTest;
         if(CheckFirstTwoAmex())
         {
            return true;
         }
         else if (CheckFirstTwoVisa())
         {
            return true;
         }
         break;
      case 15:
         firstDigitTest = creditCardNumber / FithteenDigitTest;
         if(CheckFirstTwoAmex())
         {
            return true;
         }
         break;
      case 16:
         firstDigitTest = creditCardNumber / SixteenDigitTest;
         if(CheckFirstTwoVisa())
         {
            return true;
         }
         else if (CheckFirstTwoMasterCard())
         {
            return true;
         }
      default:
         break;
   }

   //If switch doesn't return true, user has failed
   UserFailedTest();
   return false;
   
}

bool CheckFirstTwoAmex()
{
   if(firstDigitTest >= 3.4 && firstDigitTest < 3.5)
   {
      return true;
   }
   else if (firstDigitTest >= 3.7 && firstDigitTest < 3.8)
   {
      return true;
   }
   else
   {
      return false;
   }
}

bool CheckFirstTwoVisa()
{
   if (firstDigitTest >= 4 && firstDigitTest < 5)
   {
      return true;
   }
   else
   {
      return false;
   }
}

bool CheckFirstTwoMasterCard()
{
   if(firstDigitTest >= 5.1 && firstDigitTest < 5.6)
   {
      return true;
   }
   else
   {
      return false;
   }
}

bool CheckLuhnsTest()
{
   int sumOfDigits = 0;

   //Converts long to char *
   sprintf(cardAsString, "%ld", creditCardNumber);

   //Applies Luhns Algorithm (add and multiply digits) dependant on card number lenght
   if(cardNumberLength == 16)
   {
      sumOfDigits += AddAlternateDigits(0, true);
      sumOfDigits += AddAlternateDigits(1, false);
   }
   else
   {
      sumOfDigits += AddAlternateDigits(1, true);
      sumOfDigits += AddAlternateDigits(0, false);
   }

   if(sumOfDigits % 10 == 0)
   {
      return true;
   }
   else
   {
      UserFailedTest();
      return false;
   }
}

//Used to add every other digit in credit card number and double if required
int AddAlternateDigits(int startingDigit, bool doubleAnswers)
{
   int runningTotal = 0;

   for(int i = startingDigit; i < cardNumberLength; i = i + 2)
   {
      //Converts relevant char of card number to an int
      int x = cardAsString[i] - '0';
      
      if (doubleAnswers)
      {
         int answerDoubled = x * 2;
         
         //If the doubled answer has more than two digits, adds those individual digits. 
         if (x * 2 > 9)
         {
            runningTotal += answerDoubled - 9;
         }
         else
         {
            runningTotal += answerDoubled;
         }
      }
      else
      {
         runningTotal += x;
      }
   }

   return runningTotal;
}

void DisplayCardType()
{
   if(firstDigitTest < 4)
   {
      printf("AMEX\n");
   }
   else if(firstDigitTest >= 5)
   {
      printf("MASTERCARD\n");
   }
   else
   {
      printf("VISA\n");
   }
}
void UserFailedTest()
{
   printf("INVALID\n");
}