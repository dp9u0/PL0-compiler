/**********************************************************
 *file name :parser.h
 *describe : do parser transcode works
 *copyright :written by dempkwok
 *time : 2013.6.10
 **********************************************************
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "glodef.h"
/*

#define MAX_SYMBOL_TABLE_SIZE 500
#define MAX_STACK_HEIGHT 2000
#define MAX_CODE_LENGTH 500
#define MAX_LEXI_LEVELS 3

//struct define


typedef struct  {
    int     kind;       // const = 1, var = 2, proc = 3
    int    	name;  			// name up to 11 chars
    int     val;        // number (ASCII value)
    int     level;      // L level
    int     addr;       // M address
} symbol;

//used to store instructions in the internal code array
typedef struct {
    int     op;
    int     l;
    int     m;
} instruct;

//function declaration
int parser(int printFlag);
char*       tokenToString   (int token);
int         block           (int token);
int         getToken        (void);

int         statement       (int token);
int         condition       (int token);
int         expression      (int token);
int         term            (int token);
int         factor          (int token);

void        emit            (int op, int l, int m);


//identifier declaration and define
symbol      symbol_table[MAX_SYMBOL_TABLE_SIZE];
int         symbolCounter=  0;
int         fetchCounter=   0;

int         inputTable[MAX_SYMBOL_TABLE_SIZE];
int         inputCounter=   0;

instruct    code[MAX_SYMBOL_TABLE_SIZE*3];
int         codeCounter=    0;
int         stackCounter=   0;


int         sum =           0;
int         saveCondition;

int         stackCounters[MAX_LEXI_LEVELS];
int         numLexemes=     0;
int         currentLevel=   0;
int         procedureLevel= 0;
int         jumpLocation[MAX_LEXI_LEVELS];

int			doThenCheck = 0;



FILE* input;
FILE* output;
FILE* genout;



int printFlag;*/

int parser(int print)
{
    int i;
    int temp;
    printFlag= print;
    int ident[MAX_IDENT_NUM];

    input = fopen("scanout.txt","r");
    output= fopen("parseout.txt","w");
    genout= fopen("genout.txt", "w");


    //read in the input from file. here, we're just going to do a dumb print. no checking
    while (fscanf(input, "%d", &temp) != EOF)
    {
        inputTable[inputCounter]= temp;
 
        inputCounter++;
 
        //if there's an identifier, handle that special case
        if (temp == identsym)
        {
            fscanf(input, "%d", &temp);
            inputTable[inputCounter] = temp;
            inputCounter++;
            fprintf(output,"identsym.%d ", temp);
 
            if (printFlag ==1)
                printf("identsym.%d ", temp);
 
            //since we handled this, no need for more printing
            continue;
        }
        if (temp == constsym||temp == beginsym || temp ==intsym ||temp == thensym || temp ==periodsym || temp == dosym || temp==commasym ||temp == semicolonsym)
        {
            fprintf(output, "%s \n", tokenToString(temp));
 
            if (printFlag ==1)
                printf("%s \n", tokenToString(temp));
 
            //since we handled this, no need for more printing
            continue;
        }
 
        //if there's a number literal, handle that special case
        else if (temp == numbersym)
        {
            fscanf(input, "%d", &temp);
            inputTable[inputCounter] = temp;
            inputCounter++;
            fprintf(output,"numbersym.%d ", temp);
 
            if (printFlag ==1)
                printf("numbersym.%d ", temp);
 
            //since we handled this, no need for more printing
            continue;
        }
 
        //otherwise, pass the input we read to the toString function and print it
        fprintf(output, "%s ", tokenToString(temp));
 
        if (printFlag ==1)
            printf("%s ", tokenToString(temp));
 
    }
    block(getToken());
 
    //still initialized to whatever the last thing we read in was, this is why it works
    if (temp !=  periodsym)
    {
 
        fprintf(output,"Expected period at end of program file\n");
 
        if (printFlag ==1)
        {
            printf("Expected period at end of program file\n");
            printf("Press any key to continue") ;
            getchar();
            //system("PAUSE");
        }
        exit(EXIT_FAILURE);
    }
 
    //if we reach this, everthing is correct! hooray!
 
    //fprintf(output,"\n\nNo errors, program is syntactically correct\n");
 
    if (printFlag ==1)
        printf("\n\nNo errors, program is syntactically correct\n");
 
    //print out generated code to submit to the VM
    printf("\n------GENERATED CODE------\n\n");
    for(i = 0; i<codeCounter; i++)
    {
        fprintf(genout, "%d %d %d\n\n", code[i].op, code[i].l, code[i].m);
 
        if (printFlag ==1)
            printf("%d %d %d\n\n", code[i].op, code[i].l, code[i].m);
 
    }
 
    fclose(input);
    fclose(output);
    fclose(genout);
 
    return 0;
}
 
int block(int token)
{
    //3 emits for proper codeCounter for SP,BP, and RA
    emit(inc,0,1);
    emit(inc,0,1);
    emit(inc,0,1);
    //handle constants
    if (token == constsym)
    {
        symbol newConst;
        do
        {
            //fetch what should be an identifier numbering from scanner
            token= getToken();
            //assign the type of the identifier
            newConst.kind= 1;
 
            if (token != identsym)
            {
                fprintf(output, "const, int, procedure must be followed by identifier.\n");
 
                if (printFlag ==1)
                {
                    printf("const, int, procedure must be followed by identifier.\n");
                    printf("Press any key to continue...\n") ;
                    getchar();
                    //system("PAUSE");
                }
                exit(EXIT_FAILURE);
            }
 
            //fetch identifer name (an integer)
            token = getToken();
            if(findinsymbol_table(token)==1)
            {
                fprintf(output, "Found a repeated identifier in const defination block .\n");
 
                if (printFlag ==1)
                {
                    printf("Found a repeated identifier in const defination block .\n");
                    printf("Press any key to continue...\n") ;
                    getchar();
                    //system("PAUSE");
                }
                exit(EXIT_FAILURE);
            }
            newConst.name = token;
 
            token= getToken();
 
            if (token != eqlsym)
            {
                fprintf(output, "identifier must be followed by equals symbol\n");
 
                if (printFlag ==1)
                {
                    printf("identifier must be followed by equals symbol\n");
 
                    printf("Press any key to continue...\n") ;
                    getchar();
                    //system("PAUSE");
                }
                exit(EXIT_FAILURE);
            }
 
            token= getToken();
            if (token != numbersym)
            {
                fprintf(output,"equals symbol must be followed by a number\n");
 
                if (printFlag ==1)
                {
                    printf("equals symbol must be followed by a number\n");
                    printf("Press any key to continue...\n") ;
                    getchar();
                    //system("PAUSE");
                }
                exit(EXIT_FAILURE);
 
            }
 
            token = getToken();
            newConst.val= token;
 
            //store the sumbol in the symbol table and increment the counter
            symbol_table[symbolCounter] = newConst;
            symbol_table[symbolCounter].addr = stackCounter;
            symbol_table[symbolCounter].level= currentLevel;
            symbolCounter++;
 
            token= getToken();
        }
        while (token == commasym);   //run until we meet something that isn't a comma
        if (token != semicolonsym)
        {
            fprintf(output, "Semicolon or comma missing.\n");
 
 
            if (printFlag ==1)
            {
                printf("Semicolon or comma missing.\n");
                printf("Press any key to continue...\n") ;
                getchar();
                //system("PAUSE");
            }
            exit(EXIT_FAILURE);
 
        }
        token= getToken();
    }
    //found an integer
    if (token == intsym)
    {
        symbol newInt;
        newInt.kind=2;
        do
        {
            token= getToken();
            if (token != identsym)
            {
                fprintf(output, "const, int, procedure must be followed by identifier.\n");
 
                if (printFlag ==1)
                {
                    printf("const, int, procedure must be followed by identifier.\n");
                    printf("Press any key to continue...\n") ;
                    getchar();
 
                    //system("PAUSE");
                }
                exit(EXIT_FAILURE);
 
            }
            //store everything to symbol
            token = getToken();
            if(findinsymbol_table(token)==1)
            {
                fprintf(output, "Found a repeated identifier in integer defination block .\n");
 
                if (printFlag ==1)
                {
                    printf("Found a repeated identifier in integer defination block .\n");
                    printf("Press any key to continue...\n") ;
                    getchar();
                    //system("PAUSE");
                }
                exit(EXIT_FAILURE);
            }
            newInt.name = token;
            newInt.val = 0;
            newInt.addr = stackCounter;
 
            //make space on stack
            emit(inc, 0 ,1);
 
 
            //add symbol to symbol table
            symbol_table[symbolCounter] = newInt;
            symbol_table[symbolCounter].level= currentLevel;
            symbolCounter++;
 
 
            token= getToken();
        }
        while (token == commasym);
        if (token != semicolonsym)
        {
            fprintf(output, "Semicolon or comma missing.\n");
            if (printFlag ==1)
            {
                printf("Semicolon or comma missing.\n");
                printf("Press any key to continue...\n") ;
                getchar();
                //system("PAUSE");
            }
            exit(EXIT_FAILURE);
 
        }
        token= getToken();
    }
    //handles procedures
    if (token == procsym)
    {
        symbol newProc;
        jumpLocation[procedureLevel] = codeCounter;
        //emit a jump that we will edit later to skip all the declaration code
        emit(jmp, 0, 0);
 
 
        newProc.kind=3;
 
        //should be ident
        token= getToken();
        if (token != identsym)
        {
            fprintf(output, "procedure declaration must be followed by identifier\n");
 
            if (printFlag ==1)
            {
                printf("procedure declaration must be followed by identifier\n");
                printf("Press any key to continue...\n") ;
                getchar();
                //system("PAUSE");
            }
            exit(EXIT_FAILURE);
 
        }
        //this is the name of the proc
        token = getToken();
        if(findinsymbol_table(token)==1)
        {
            fprintf(output, "Found a repeated identifier in procedure defination block .\n");
 
            if (printFlag ==1)
            {
                printf("Found a repeated identifier in procedure defination block .\n");
                printf("Press any key to continue...\n") ;
                getchar();
                //system("PAUSE");
            }
            exit(EXIT_FAILURE);
        }
 
        newProc.name=token;
        newProc.addr= codeCounter;
 
 
        symbol_table[symbolCounter] = newProc;
        symbol_table[symbolCounter].level= currentLevel;
        symbolCounter++;
 
 
        //this should be the semicolon
        token= getToken();
        if (token != semicolonsym)
        {
            fprintf(output, "Semicolon missing after procedure declaration\n");
 
            if (printFlag ==1)
                printf("Semicolon missing after procedure declaration\n");
 
            exit(EXIT_FAILURE);
 
        }
        //this is the first token after the declaration, the start of the function definition
        token= getToken();
 
        //incremement the lexeme counter because now we are looking at nested procedure declarations
        currentLevel++;
        procedureLevel++;
 
        //since we're working in a new procedure at this point, set the stack counter to zero. store
        //current stack value so we can get it back
        stackCounters[numLexemes] = stackCounter;
        numLexemes++;
        stackCounter= 0;
 
        //generate code for the procedure
        token= block(token);
 
        //now that we've generated code for this procedure, return to the actual stack values
        stackCounters[numLexemes]=0;
        numLexemes--;
        stackCounter= stackCounters[numLexemes];
 
        if (token != semicolonsym)
        {
            fprintf(output, "Semicolon missing at end of procedure description\n");
 
            if (printFlag ==1)
            {
                printf("Semicolon missing at end of procedure description\n");
                printf("Press any key to continue...\n") ;
                getchar();
                //system("PAUSE");
            }
            exit(EXIT_FAILURE);
 
        }
        //set up next token
        token= getToken();
 
        procedureLevel--;
 
        //edit the jump to skip it all
        code[jumpLocation[procedureLevel]].m = codeCounter;
    }
 
    //after every declaration, do work
    token = statement(token);
 
    //decrement current level, we need to get out of the recursion --l0
    currentLevel--;
 
    return token;
}
 
int statement(int token)
{
    //global counter, tells where we are in the code list
    int codeIndex;
    if (token == identsym)
    {
        //we found an identsym, now get that identifier's name
        token = getToken();
        //make sure if this identifier has been declared
        if(findinsymbol_table(token)==0)
        {
            fprintf(output, "Undefined identifier .\n");
 
            if (printFlag ==1)
            {
                printf("Undefined identifier .\n");
                printf("Press any key to continue...\n") ;
                getchar();
                //system("PAUSE");
            }
            exit(EXIT_FAILURE);
        }
        for(codeIndex = 0; codeIndex< symbolCounter; codeIndex++)
        {
            if(symbol_table[codeIndex].name == token && symbol_table[codeIndex].kind == 2)
            {
                break; //if we found the identifier in our symbol table, stop looping. we will use this index from here out.
            }
        }
        token= getToken();
        if (token != becomessym)
        {
            fprintf(output, "Incorrect symbol following declaration statement\n");
 
            if (printFlag ==1)
            {
                printf("Incorrect symbol following declaration statement\n");
                printf("Press any key to continue...\n") ;
                getchar();
                //system("PAUSE");
            }
            exit(EXIT_FAILURE);
 
        }
 
        token= getToken();
 
        token = expression(token);
        emit(sto,currentLevel - symbol_table[codeIndex].level ,symbol_table[codeIndex].addr);
 
    }
    //Runs when the call command is executed.
    else if (token == callsym)
    {
        stackCounters[numLexemes] = stackCounter;
        numLexemes++;
        stackCounter= 0;
 
        token= getToken();
        if (token != identsym)
        {
            fprintf(output, "call must be followed by an identifier\n");
 
            if (printFlag ==1)
            {
                printf("call must be followed by an identifier\n");
                printf("Press any key to continue...\n") ;
                getchar();
                //system("PAUSE");
            }
            exit(EXIT_FAILURE);
 
        }
        //we found an identsym, now get that identifier's name
        token = getToken();
        if(findinsymbol_table(token)==0)
        {
            fprintf(output, "Undefined identifier .\n");
 
            if (printFlag ==1)
            {
                printf("Undefined identifier .\n");
                printf("Press any key to continue...\n") ;
                getchar();
                //system("PAUSE");
            }
            exit(EXIT_FAILURE);
        }
        for(codeIndex = 0; codeIndex< symbolCounter; codeIndex++)
        {
            if(symbol_table[codeIndex].name == token && symbol_table[codeIndex].kind == 3)
            {
                break; //if we found the identifier in our symbol table, stop looping. we will use this index from here out.
            }
        }
        emit(cal, currentLevel- symbol_table[codeIndex].level, symbol_table[codeIndex].addr);
        token= getToken();
    }
 
    //found begin
    else if (token == beginsym)
    {
        //find first element after begin
        token= getToken();
        //do work
        token= statement(token);
        do
        {
            //found the end declaration of function. don't do more work
 
 
            token= getToken();
 
            //do more work
            token= statement(token);
 
            if (token == endsym)
            {
                if(doThenCheck > 0)
                {
                    continue;
                }
 
                else
                {
                    //return control to calling function
                    emit(opr, 0,0);
 
                    if (numLexemes > 0)
                    {
                        stackCounters[numLexemes]=0;
                        numLexemes--;
                        stackCounter= stackCounters[numLexemes];
                    }
 
                    continue;
                }
            }
 
        }
        while (token == semicolonsym);
        if (token != endsym)
        {
            fprintf(output, "end expected\n");
 
            if (printFlag ==1)
                printf("end expected\n");
            printf("Press any key to continue...\n") ;
            getchar();
            //system("PAUSE");
 
            exit(EXIT_FAILURE);
        }
 
        token= getToken();
    }
 
    else if (token == ifsym)
    {
        int ctemp;
        int tempToken;
        doThenCheck++;
        //get ready to swap
        token= getToken();
 
        //do work
        token= condition(token);
 
 
        if (token != thensym)
        {
            fprintf(output, "then expected\n");
 
            if (printFlag ==1)
            {
                printf("then expected\n");
                printf("Press any key to continue...\n") ;
                getchar();
                //system("PAUSE");
            }
            exit(EXIT_FAILURE);
 
        }
 
        token = getToken();
 
        //set temp to where code counter is in code list
        ctemp = codeCounter;
        emit(jpc, 0, 0); //put jpc in code list
 
        //do work
        token= statement(token);
 
        //set jump to actual location where we need ot jump to
        code[ctemp].m = codeCounter;
 
        tempToken = getToken();
        if(tempToken == elsesym)
        {
            doThenCheck++;
            code[ctemp].m = codeCounter+1;
            token = tempToken;
            ctemp = codeCounter;
            emit(jmp, 0 , 0);
            token = getToken();
            token = statement(token);
            code[ctemp].m = codeCounter;
            doThenCheck--;
        }
        else
        {
            fetchCounter--;
        }
        doThenCheck--;
    }
 
    else if (token == whilesym)
    {
        int cx2;
        //store where code counter is before we do work
        int cx1 =codeCounter;
        doThenCheck++;
        token= getToken();
 
        //do work
        token= condition(token);
 
        //store where we count after work is done
        cx2 = codeCounter;
 
        emit(jpc, 0, 0);
        if (token != dosym)
        {
            fprintf(output, "do expected\n");
 
            if (printFlag ==1)
            {
                printf("do expected\n");
                printf("Press any key to continue...\n") ;
                getchar();
                //system("PAUSE");
            }
            exit(EXIT_FAILURE);
        }
        token= getToken();
 
        token= statement(token);
 
        //emit a new jump to redo code to while
        emit(jmp, 0, cx1);
 
        //edit previous jpc with new count value
        code[cx2].m = codeCounter;
        doThenCheck--;
    }
 
 
    else if(token == insym)
    {
        token = getToken();
        if (token != identsym)
        {
            fprintf(output, "call must be followed by an identifier\n");
 
            if (printFlag ==1)
            {
                printf("call must be followed by an identifier\n");
                printf("Press any key to continue...\n") ;
                getchar();
                //system("PAUSE");
            }
            exit(EXIT_FAILURE);
 
        }
 
        token = getToken();
        if(findinsymbol_table(token)==0)
        {
            fprintf(output, "Undefined identifier .\n");
 
            if (printFlag ==1)
            {
                printf("Undefined identifier .\n");
                printf("Press any key to continue...\n") ;
                getchar();
                //system("PAUSE");
            }
            exit(EXIT_FAILURE);
        }
 
        emit(10, 0,2);
        token = getToken();
    }
 
    else if(token == outsym)
    {
        token= getToken();
 
        //pass expression to the proper funciton
        token= expression(token);
 
        emit(sio, 0,1);
    }
    return token;
}
 
//this is called when we encounter a conditional and need to evaluate it
int condition(int token)
{
 
    //if it's an oddsym, just pass it to expression
    if (token == oddsym)
    {
        token= getToken();
        token= expression(token);
    }
    else
    {
        token= expression(token);
        if (token != eqlsym && token != neqsym && token != lessym && token != leqsym && token != gtrsym && token != geqsym)
        {
            fprintf(output, "relational operator expected\n");
 
            if (printFlag ==1)
            {
                printf("relational operator expected\n");
                printf("Press any key to continue...\n") ;
                getchar();
                //system("PAUSE");
            }
 
            exit(EXIT_FAILURE);
 
        }
        //sterilize global
        saveCondition= 0;
 
        //enter switch to determine what the conditional is
        switch (token)
        {
        case eqlsym:
        {
            saveCondition= EQL;
            break;
        }
 
        case neqsym:
        {
            saveCondition=NEQ;
            break;
        }
 
        case lessym:
        {
            saveCondition= LSS;
            break;
        }
 
        case leqsym:
        {
            saveCondition= LEQ;
            break;
        }
 
        case gtrsym:
        {
            saveCondition= GTR;
            break;
        }
 
        case geqsym:
        {
            saveCondition= GEQ;
            break;
        }
 
        }
        token= getToken();
 
        //pass expression to the proper funciton
        token= expression(token);
 
        //emit operator with proper oprcode
        emit(opr, 0, saveCondition);
    }
 
 
    return token;
}
 
//evaluate math expressions
int expression(int token)
{
    int addop;
 
    //addition and subtraction
    if (token == plussym || token == minussym)
    {
        addop = token;
        token= getToken();
        token= term(token);
 
        //handle negatives
        if(addop == minussym)
            emit(opr, 0, NEG); // negate
    }
    //send to term function
    else
        token= term(token);
 
    //run until we run out of operators
    while (token == plussym || token == minussym)
    {
        addop = token;
        token= getToken();
        
        token= term(token);
        if (addop == plussym)
            emit(opr, 0, ADD); // addition
        else
            emit(opr, 0, SUB); // subtraction
    }
 
    return token;
}
 
//term handles multiplication and division
int term(int token)
{
	    if(findinsymbol_table(token)==0)
    {
        fprintf(output, "Undefined identifier .\n");
 
        if (printFlag ==1)
        {
            printf("Undefined identifier .\n");
            printf("Press any key to continue...\n") ;
            getchar();
            //system("PAUSE");
        }
        exit(EXIT_FAILURE);
    }
	
    int mulop;
    token= factor(token);
 
    //while we still have terms
    while(token == multsym || token == slashsym)
    {
        mulop = token;
        token= getToken();
        token= factor(token);
        if(mulop == multsym)
            emit(opr, 0, MUL); // multiplication
        else
            emit(opr, 0, DIV); // division
    }
 
    return token;
}
 
//handles individual factors of expressions
int factor(int token)
{
    int j;
    //if you find a variable
    if (token == identsym)
    {
        token = getToken();
 
        //fetch the value of the identifier from the symbol table
        for(j = 0; j< symbolCounter; j++)
        {
            if(symbol_table[j].name == token)
            {
 
                //if it's a constant, emit the lit
                if (symbol_table[j].kind == 1)
                {
                    emit(lit,0, symbol_table[j].val);
                    break;
                }
                //else, load up the address of the factor and emit it for the vm.
                else
                {
                    emit(lod, currentLevel -  symbol_table[j].level , symbol_table[j].addr);
                    break;
                }
            }
        }
        token= getToken();
    }
    //if we find a numbersym, just emit the number literal
    else if (token == numbersym)
    {
        token = getToken();
        emit(lit, 0, token);
        token= getToken();
    }
    //if we find a left parenthesis, evaluate as a new expression.
    else if (token == lparentsym)
    {
        token= getToken();
        token= expression(token);
        if (token != rparentsym)
        {
            fprintf(output, "right parenthesis missing.\n");
            if (printFlag ==1)
            {
                printf("right parenthesis missing.\n");
                printf("Press any key to continue...\n") ;
                getchar();
                //system("PAUSE");
            }
 
            exit(EXIT_FAILURE);
 
        }
        token= getToken();
    }
    else
    {
        fprintf(output, "invalid factor found\n");
        if (printFlag ==1)
        {
            printf("invalid factor found\n");
            printf("Press any key to continue...\n") ;
            getchar();
            //system("PAUSE");
        }
 
        exit(EXIT_FAILURE);
 
    }
 
    return token;
}
 
int getToken(void)
{
    //get next token from the input table and then increment the global counter
    int temp= inputTable[fetchCounter];
    fetchCounter++;
 
    return temp;
}
 
void emit(int op, int l, int m)
{
    //if the code counter has gone above the maximum allowable code length, error out
    if(codeCounter > MAX_CODE_LENGTH)
    {
        fprintf(output, "code generation overflow\n");
 
        if (printFlag ==1)
        {
            printf("code generation overflow\n");
            printf("Press any key to continue...\n") ;
            getchar();
            //system("PAUSE");
        }
        exit(EXIT_FAILURE);
    }
    //otherwise add the passed args to the code array
    else
    {
        code[codeCounter].l  = l;   // lexicographical level
        code[codeCounter].op = op;  //opcode
        code[codeCounter].m  = m;   //modifier
        codeCounter++;
 
        stackCounter++;
    }
 
    if (op == sto)
        stackCounter-= 1;
}
 
//a function that takes in integer value and returns a string of the token name to print
char* tokenToString(int token)
{
    switch (token)
    {
    case (int) plussym:
        return "plussym";
 
    case (int) minussym:
        return "minussym";
 
    case (int) numbersym:
        return "numbersym";
 
    case (int) multsym:
        return "multsym";
 
    case (int) slashsym:
        return "slashsym";
 
    case (int) lessym:
        return "lesssym";
 
    case(int) eqlsym:
        return "eqlsym";
 
    case (int) gtrsym:
        return "gtrsym";
 
    case(int) geqsym:
        return "geqsym";
 
    case(int) leqsym:
        return "leqsym";
 
    case(int) becomessym:
        return "becomessym";
 
    case(int) neqsym:
        return "neqsym";
 
    case (int) lparentsym:
        return "lparentsym";
 
    case (int) rparentsym:
        return "rparentsym";
 
    case (int) commasym:
        return "commasym";
 
    case (int) semicolonsym:
        return "semicolonsym";
 
    case (int) periodsym:
        return "periodsym";
 
    case (int) intsym:
        return "intsym";
 
    case (int) beginsym:
        return "beginsym";
 
    case (int) identsym:
        return "identsym";
 
    case (int) endsym:
        return "endsym";
 
    case (int) ifsym:
        return "ifsym";
 
    case (int) thensym:
        return "thensym";
 
    case (int) whilesym:
        return "whilesym";
 
    case (int) dosym:
        return "dosym";
 
    case (int) callsym:
        return "callsym";
 
    case (int) constsym:
        return "constsym";
 
    case (int) elsesym:
        return "elsesym";
 
    case (int) procsym:
        return "procsym";
 
    case (int) insym:
        return "in";
 
    case (int) outsym:
        return "outsym";
 
    case (int) nulsym:
        return "NULL";
 
    default:
        return "ERR";
    }
}
 
int findinsymbol_table(int iname)
{
    int count=0;
 
    for(count=0; count<=symbolCounter; count++)
    {
        if(symbol_table[count].name==iname)
            return 1;
    }
    return 0;
}
 