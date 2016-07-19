/**********************************************************
 *file name :scanner.h
 *describe : read source file and turn them to tokens
 *copyright :written by dempkwok
 *time : 2013.6.10
 **********************************************************
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "glodef.h"

/*#define IDENTIFIER_MAX_LENGTH   11
#define NUM_MAX_LENGTH          5

//enum as defined for all symbols
typedef enum {
    nulsym = 1, 	identsym = 2, 	numbersym = 3, 		plussym = 4,	minussym = 5,
    multsym = 6,  	slashsym = 7, 	oddsym = 8, 		eqlsym = 9, 	neqsym = 10,
    lessym = 11, 	leqsym = 12, 	gtrsym = 13, 		geqsym = 14, 	lparentsym = 15,
    rparentsym = 16,commasym = 17, 	semicolonsym = 18, 	periodsym = 19, becomessym = 20,
    beginsym = 21,  endsym = 22, 	ifsym = 23, 		thensym = 24, 	whilesym = 25,
    dosym = 26,		callsym = 27, 	constsym = 28, 		intsym = 29, 	procsym = 30,
    outsym = 31, 	insym = 32, 	elsesym = 33, 		colonsym = 34
} token_type;

int scanner(int printFlag);

int charToTokenType(char symbol);
int tokenizeReservedWord(char* word, int* i);
char* tokenDeparser(int token);

FILE* input;
FILE* output;

char    inputFile           [20000];
int     fileLength=         0;

int     tokens              [20000];
int     tokenCounter=       0;

int     numbers             [20000];
int     numberCounter=      0;
int     numberPrintCount=   0;

char    identifiers         [20000][IDENTIFIER_MAX_LENGTH];
int     intIdentifiers      [20000];
int     identifierIndex=    0;
int     uniqueIdentifiers=  1;

int     printFlag;*/

int scanner(int print)
{
    printFlag= print;

    char temp;
    int i;
    input=    fopen("input.txt", "r");
    output=   fopen("scanout.txt", "w");  
    i=0;
    //read in the file char by char until we run out of file
    while(fscanf(input,"%c", &temp) != EOF)
    {
        inputFile[i]= temp;
        i++;
        fileLength++;
    }
 
    //this loop runs per character, and if you alter your character index in any way you must use continue
    for (i=0; i< fileLength; i++)
    {
        //removes comments
        if (inputFile[i] == '/' && inputFile[i+1] == '*')
        {
            //don't consider the /
            int j=i+1;
 
            //run until we encounter a * next to /
            while (!(inputFile[j] == '*' && inputFile[j+1] == '/'))
            {
                j++;
            }
 
            //increment i so we skip whatever was in there, and start back up after the /
            i=j+1;
            continue;
        }
 
        //removes spaces
        if (inputFile[i] == ' ')
            continue;
 
        //removes newlines
        if (inputFile[i] == '\n')
            continue;
 
        //removes tabs
        if (inputFile[i] == '\t')
            continue;
 
        //handles reserved words
        if (inputFile[i] >= 'a' && inputFile[i] <= 'z')
        {
            switch ((int) inputFile[i])
            {
                //begin
            case (int) 'b':
                if ((inputFile[i+1] == 'e') &&
                        (inputFile[i+2] == 'g') &&
                        (inputFile[i+3] == 'i') &&
                        (inputFile[i+4] == 'n'))
                {
                    i+=4;
                    tokens[tokenCounter] = beginsym;
                    tokenCounter++;
                    continue;
                }
 
                //end
            case (int) 'e':
                if ((inputFile[i+1] == 'n') &&
                        (inputFile[i+2] == 'd'))
                {
                    i+=2;
                    tokens[tokenCounter] = endsym;
                    tokenCounter++;
                    continue;
                }
                //else
                else if ((inputFile[i+1] == 'l') &&
                         (inputFile[i+2] == 's') &&
                         (inputFile[i+3] == 'e'))
                {
                    i+=3;
                    tokens[tokenCounter] = elsesym;
                    tokenCounter++;
                    continue;
                }
                //if ,int, in
            case (int) 'i':
                if (inputFile[i+1] == 'f') //if
                {
                    i+=1;
                    tokens[tokenCounter] = ifsym;
                    tokenCounter++;
                    continue;
                }
 
                //int
                else if((inputFile[i+1] == 'n') && (inputFile[i+2] == 't'))
                {
                    i+=2;
                    tokens[tokenCounter] = intsym;
                    tokenCounter++;
                    continue;
                }
 
                //in   ----read
                else if ((inputFile[i+1] == 'n'))
                {
                    i+=1;
                    tokens[tokenCounter] = insym;
                    tokenCounter++;
                    continue;
                }
 
 
                //then
            case (int) 't':
                if ((inputFile[i+1] == 'h') &&
                        (inputFile[i+2] == 'e') &&
                        (inputFile[i+3] == 'n'))
                {
                    i+=3;
                    tokens[tokenCounter] = thensym;
                    tokenCounter++;
                    continue;
                }
                //while
            case (int) 'w':
                if ((inputFile[i+1] == 'h') &&
                        (inputFile[i+2] == 'i') &&
                        (inputFile[i+3] == 'l') &&
                        (inputFile[i+4] == 'e'))
                {
                    i+=4;
                    tokens[tokenCounter] = whilesym;
                    tokenCounter++;
                    continue;
                }
 
                //do
            case (int) 'd':
                if (inputFile[i+1] == 'o')
                {
                    i+=1;
                    tokens[tokenCounter] = dosym;
                    tokenCounter++;
                    continue;
                }
                //call
            case (int) 'c':
                if ((inputFile[i+1] == 'a') &&
                        (inputFile[i+2] == 'l') &&
                        (inputFile[i+3] == 'l'))
                {
                    i+=3;
                    tokens[tokenCounter] = callsym;
                    tokenCounter++;
                    continue;
                }
                //const
                else if ((inputFile[i+1] == 'o') &&
                         (inputFile[i+2] == 'n') &&
                         (inputFile[i+3] == 's') &&
                         (inputFile[i+4] == 't'))
                {
                    i+=4;
                    tokens[tokenCounter] = constsym;
                    tokenCounter++;
                    continue;
                }
            case (int) 'o':
 
                //odd
                if ((inputFile[i+1] == 'd') &&
                        (inputFile[i+2] == 'd'))
                {
                    i+=2;
                    tokens[tokenCounter] = oddsym;
                    tokenCounter++;
                    continue;
                }
 
                //out
                else if ((inputFile[i+1] == 'u') &&
                         (inputFile[i+2] == 't'))
                {
                    i+=2;
                    tokens[tokenCounter] = outsym;
                    tokenCounter++;
                    continue;
                }
 
                //procedure
            case (int) 'p':
                if ((inputFile[i+1] == 'r') &&
                        (inputFile[i+2] == 'o') &&
                        (inputFile[i+3] == 'c') &&
                        (inputFile[i+4] == 'e') &&
                        (inputFile[i+5] == 'd') &&
                        (inputFile[i+6] == 'u') &&
                        (inputFile[i+7] == 'r') &&
                        (inputFile[i+8] == 'e'))
                {
                    i+=8;
                    tokens[tokenCounter] = procsym;
                    tokenCounter++;
                    continue;
                }
 
            default:
                break;
            }
        }
 
        //handles symbols. the function below takes in a char and returns it into atoken
        temp= charToTokenType(inputFile[i]);
        if (temp != 0)
        {
            int secondSymbol;
            //Check for Double symbol of not equal <> or 'less than or equal '<=
            if(temp == lessym)
            {
                secondSymbol = charToTokenType(inputFile[i+1]);
                if(secondSymbol == gtrsym)
                {
                    tokens[tokenCounter] = neqsym;
                    tokenCounter++;
                    i++;
                    continue;
                }
                else if(secondSymbol == eqlsym)
                {
                    tokens[tokenCounter] = leqsym;
                    tokenCounter++;
                    i++;
                    continue;
                }
            }
 
            //Check for double symbol greater than or equal >=
            if(temp == gtrsym)
            {
                secondSymbol = charToTokenType(inputFile[i+1]);
                if(secondSymbol == eqlsym)
                {
                    tokens[tokenCounter] = geqsym;
                    tokenCounter++;
                    i++;
                    continue;
                }
            }
 
            //check for becomes :=
            if(temp == colonsym)
            {
                secondSymbol = charToTokenType(inputFile[i+1]);
                if(secondSymbol == eqlsym)
                {
                    tokens[tokenCounter] = becomessym;
                    tokenCounter++;
                    i++;
                    continue;
                }
                //error out
                else
                {
                    fprintf(output, "\n\nerror: invalid symbol!");
                    exit(EXIT_FAILURE);
                }
            }
 
            secondSymbol = charToTokenType(inputFile[i+1]);
 
            //add the token we found to the token array we're storing
            tokens[tokenCounter] = temp;
            tokenCounter++;
 
            continue;
        }
 
        //identifiers
        if ((inputFile[i] >= 'A' && inputFile[i] <= 'Z') ||(inputFile[i] >= 'a' && inputFile[i] <= 'z'))
        {
            int v;
            int flag= 1;
            int Identifier_Length;
            char Identifier [IDENTIFIER_MAX_LENGTH];
            int j;
            int k;
            //sanitize string
            for (k=0; k< IDENTIFIER_MAX_LENGTH; k++)
                Identifier[k]= '\0';
 
            Identifier_Length=0;
 
            j=i;
 
            while((inputFile[j] >= 'A' && inputFile[j] <= 'Z') ||
                    (inputFile[j] >= 'a' && inputFile[j] <= 'z') ||
                    (inputFile[j] >= '0' && inputFile[j] <= '9'))
            {
                //identifier is too large, error out
                if (Identifier_Length > 11)
                {
                    fprintf(output, "\n\nerror: identifier too large!");
                    exit(EXIT_FAILURE);
                }
 
                //otherwise store the char inside a new string for an identifier
                Identifier[Identifier_Length] = inputFile[j];
                Identifier_Length++;
                j++;
            }
 
            //store that we have an identifier
            tokens[tokenCounter]= identsym;
            tokenCounter++;
 
 
            for (v=0; v<identifierIndex; v++)
            {
                if (strcmp(Identifier, identifiers[v]) == 0)
                {
                    intIdentifiers[identifierIndex]= v+1;
                    flag= 0;
                    break;
                }
            }
            if (flag)
            {
                intIdentifiers[identifierIndex]= uniqueIdentifiers;
                uniqueIdentifiers++;
            }
 
            //copy identifier to array
            strcpy(identifiers[identifierIndex],Identifier);
 
            //increment the counter
            identifierIndex++;
            i= j-1;
            continue; 
        }
 
        //convert identifiers to numbers
 
        //handles numbers
        if (inputFile[i] >= '0' && inputFile[i] <= '9')
        {
            int digitCount;
            int number;
            int j= i;
            int k;
            //checks length of number
            for (j= i; j< NUM_MAX_LENGTH+i +1 ; j++)
            {
 
                //if we find any whitespace after the number, end this
                if ((inputFile[j] == ' ') || (inputFile[j] == '\n') ||
                        (inputFile[j] == '\t'))
                    break;
 
                //if we find a letter
                if ((inputFile[j] >= 'A' && inputFile[j] <= 'Z') ||
                        (inputFile[j] >= 'a' && inputFile[j] <= 'z'))
                {
                    fprintf(output, "\n\nerror: invalid number in a identsym!!");
                    if (printFlag ==1)
               			 {
	                    printf("\n\nerror: invalid number in a identsym!!\n");
	 
	                    printf("Press any key to continue...\n") ;
	                    getchar();
	                    //system("PAUSE");
                			}
                    exit(EXIT_FAILURE);
                }
            }
 
            //reset j from the for loop so it works with the while loop we're about to use
            j=i;
 
            //starts at 1 because we are guaranteed to have at least one digit at this point
            digitCount=1;
            number=0;
            //checks to make sure number isn't too big
            while (inputFile[j] >= '0' && inputFile[j] <= '9')
            {
            		           		
                if (digitCount > 5)
                {
                    fprintf(output, "\n\nerror: number too large!");
                    exit(EXIT_FAILURE);
                }
                digitCount++;
                j++;
            }
 
            //needed so that it sends the digits to the proper power, down to x^0, or 1s place.
            digitCount--;
 
 
            for (k= j-digitCount; k<j; k++)
            {
                number += (inputFile[k]- '0') * pow(10.0, digitCount-1);
                digitCount--;
            }
 
            //add that we have a number to the token list
            tokens[tokenCounter]= numbersym;
            tokenCounter++;
 
            //add number to the number list
            numbers[numberCounter]= number;
            numberCounter++;
 
            i= j-1;
            continue;
        }
 
        //        //if it is nothing else, it's invalid input
        //        fprintf(output, "\n\nerror: invalid symbol!");
        //        exit(EXIT_FAILURE);
    }
 
 
    numberPrintCount= 0;
    identifierIndex= 0;
 
    for (i=0; i< tokenCounter; i++)
    {
        if (tokens[i]== numbersym)
        {
            fprintf(output, "%d %d ", tokens[i], numbers[numberPrintCount]);
            numberPrintCount++;
 
            if (printFlag ==1)
                printf("%d %d ", tokens[i], numbers[numberPrintCount]);
        }
        else if (tokens[i]== identsym)
        {
            fprintf(output, "%d %d ", tokens[i], intIdentifiers[identifierIndex]);
            identifierIndex++;
 
            if (printFlag ==1)
                printf("%d %d ", tokens[i], intIdentifiers[identifierIndex]);
        }
 
        else
        {
            fprintf(output, "%d ", tokens[i]);
            if (printFlag ==1)
                printf("%d ", tokens[i]);
        }
    }
     if (printFlag ==1)
        printf("\n\nNo errors, scanner running correct\n");
        
    fclose(input);
    fclose(output);
 
    return 0;
}
 
//converts a single char to a token type. Returns 0 if token not found.
int charToTokenType(char symbol)
{
    switch ((int) symbol)
    {
    case (int) '+':
        return plussym;
 
    case (int) '-':
        return minussym;
 
    case (int) '*':
        return multsym;
 
    case (int) '/':
        return slashsym;
 
    case (int) '<':
        return lessym;
 
    case (int) '>':
        return gtrsym;
 
    case (int) '(':
        return lparentsym;
 
    case (int) ')':
        return rparentsym;
 
    case (int) ',':
        return commasym;
 
    case (int) ';':
        return semicolonsym;
 
    case (int) '.':
        return periodsym;
 
    case (int) '=':
        return eqlsym;
 
    case (int) ':':
        return colonsym;
 
    default:
        return 0;
    }
    return 0;
}
 
char* tokenDeparser(int token)
{
    char* temp;
    switch (token)
    {
    case (int) plussym:
        return "+";
 
    case (int) minussym:
        return "-";
 
    case (int) multsym:
        return "*";
 
    case (int) slashsym:
        return "/";
 
    case (int) lessym:
        return "<";
 
    case(int) eqlsym:
        return "=";
 
    case (int) gtrsym:
        return ">";
 
    case(int) geqsym:
        return ">=";
 
    case(int) leqsym:
        return "<=";
 
    case(int) becomessym:
        return ":=";
 
    case(int) neqsym:
        return "<>";
 
    case (int) lparentsym:
        return "(";
 
    case (int) rparentsym:
        return ")";
 
    case (int) commasym:
        return ",";
 
    case (int) semicolonsym:
        return ";";
 
    case (int) periodsym:
        return ".";
 
    case (int) intsym:
        return "int";
 
    case (int) beginsym:
        return "begin";
 
    case (int) identsym:
        strcpy(temp,identifiers[uniqueIdentifiers]);
        uniqueIdentifiers++;
        return temp;
 
    case (int) endsym:
        return "end";
 
    case (int) ifsym:
        return "if";
 
    case (int) thensym:
        return "then";
 
    case (int) whilesym:
        return "while";
 
    case (int) dosym:
        return "do";
 
    case (int) callsym:
        return "call";
 
    case (int) constsym:
        return "const";
 
    case (int) elsesym:
        return "else";
 
    case (int) procsym:
        return "procedure";
 
    case (int) insym:
        return "in";
 
    case (int) outsym:
        return "out";
 
    case (int) nulsym:
        return NULL;
 
    default:
        return 0;
    }
}
 