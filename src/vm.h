/**********************************************************
 *file name :vm.h
 *describe : running code in a virsual stack 
 *copyright :written by dempkwok
 *time : 2013.6.10
 **********************************************************
 */
 
#include <stdlib.h>
#include <stdio.h>
#include "glodef.h"
 
 
/*#define MAX_STACK_HEIGHT 2000
#define MAX_CODE_LENGTH 500
#define MAX_LEXI_LEVELS 3

typedef struct {
    int op; //opcode
    int l;  // L
    int m;  // M
}instruction;

//makes enums for readability
typedef enum
{
    hlt,
    lit,
    opr,
    lod,
    sto,
    cal,
    inc,
    jmp,
    jpc,
    sio
} opcode;

typedef enum
{
    RET,
    NEG,
    ADD,
    SUB,
    MUL,
    DIV,
    ODD,
    MOD,
    EQL,
    NEQ,
    LSS,
    LEQ,
    GTR,
    GEQ
} oprcode;

typedef enum
{
    FALSE,
    TRUE
} boolean;

int vm(int printFlag);

int base            (int l, int base);
void fetch          (instruction i);

//the print functions that the program uses.
//these all output to files.
void printAsm       (instruction array[], int length);
void printInit      (instruction instruct);
void printExec      (void);
void printStack     (void);
int  printFlag;



//the functions that have opcodes assigned to them
int literal         (int L, int M);
int operation       (int L, int M);
int load            (int L, int M);
int store           (int L, int M);
int call            (int L, int M);
int increment       (int L, int M);
int jump            (int L, int M);
int jumpCondition   (int L, int M);
int stdIO           (int L, int M);
//the stack and maintenance function
int stackCheck      (int arg);


FILE* input;
FILE* output;
//boolean flag to see if we are printing on this line
int stdOutPrint     = FALSE;
//saves value to be printed out for stdIO
int stdOutReg;




int stack           [MAX_STACK_HEIGHT];
int maxStack        = 0;


//the activation record array and top counter variable
int AR[5];
int numARs          = 0;

//Initial “stack” store
int SP              = 0;
int BP              = 1;
int PC              = 0;
int IR              = 0;*/
 
int vm(int print)
{
    printFlag= print;
 
    int numInstructions=0;
    instruction instructions[MAX_CODE_LENGTH];
    input=    fopen("genout.txt", "r");
    output=   fopen("output.txt", "w");
 
 
    //read in instructions to array until EOF
    while (fscanf(input, "%d %d %d",
                  &instructions[numInstructions].op,
                  &instructions[numInstructions].l,
                  &instructions[numInstructions].m) != EOF)
    {
        //increment instruction counter
        numInstructions++;
    }
 
    //print out parsed assembly input
    printAsm(instructions, numInstructions);
 
    if (printFlag ==1)
    {
        printf("\t\t\t\t\t\t\tpc\tbp\tsp\tstack\n");
        printf("\t\t\t\t\t\t\t%2d\t%2d\t%2d\t(zeroed out)\n", PC, BP, SP);
    }
 
    //header for execution print
    fprintf(output,"\t\t\t\t\t\t\tpc\tbp\tsp\tstack\n");
    fprintf(output,"\t\t\t\t\t\t\t%2d\t%2d\t%2d\t(zeroed out)\n", PC, BP, SP);
 
    //the main loop. runs over every instruction,
    //fetches and then inside fetch executes
    do
    {
        //fetch the function
        fetch(instructions[PC]);
        //print the new values of the pointers
        printExec();
        //print out the current value of the stack
        printStack();
        //print out a formatting newline
        fprintf(output,"\n");
 
        if (printFlag ==1)
        {
            printf("\n");
        }
 
        if (stdOutPrint == TRUE)
        {
            fprintf(output,"-------------\n");
            fprintf(output,"Output: %d\n-------------\n", stdOutReg);
            stdOutPrint= FALSE;
        }
        //run until registers are empty
    }
    while (PC != 0 && BP !=0);  // && SP !=0);
 
    return 0;
}
 
/**********************************************/
/*
 Find base L levels down
 */
/**********************************************/
int base(int l, int base)
{
    int b1; //find base L levels down
    b1 = base;
    while (l > 0)
    {
        b1 = stack[b1];
        l--;
    }
    return b1;
}
 
/**********************************************/
/*
 take in an instruction and call the
 appropriate function
 */
/**********************************************/
void fetch(instruction i)
{
    printInit(i);
 
    PC++;
    switch (i.op)
    {
    case (hlt):
        //code
        break;
 
        //01 – LIT 0, M
        //Push constant value (literal) M onto the stack
    case (lit):
        literal(0, i.m);
        break;
 
        //02 – OPR 0, M
        //Operation to be performed on the data at the top of the stack
    case (opr):
        operation(0, i.m);
        break;
 
        //LOD L, M
        //Load value to top of stack from the stack location at offset
        //M from L lexicographical levels down
    case (lod):
        load(i.l, i.m);
        break;
 
        //STO L, M
        //Store value at top of stack in the stack location at offset M
        //from L lexicographical levels down
    case (sto):
        store(i.l, i.m);
        break;
 
        //CAL L, M
        //Call procedure at code index M (generates new Activation Record
        //and pc M)
    case (cal):
        call(i.l, i.m);
        break;
 
        //INC 0, M
        //Allocate M locals (increment sp by M). First three are Static Link
        //(SL), Dynamic Link (DL), and Return Address (RA)
    case (inc):
        increment(0, i.m);
        break;
 
        //JMP 0, M
        //Jump to instruction M
    case (jmp):
        jump(0, i.m);
        break;
 
        //JPC 0, M
        //Jump to instruction M if top stack element is 0
    case (jpc):
        jumpCondition(0, i.m);
        break;
 
        //SIO 0, 1
        //Write the top stack element to the screen
    case (sio):
        stdIO(0, 1);
        break;
 
        //SIO 0, 2
        //Read in input from the user and store it at the top of the stack
    case (10):
        stdIO(0, 2);
        break;
 
        //something went wrong...
    default:
        return;
    }
    maxStack= stackCheck(SP);
}
 
/**********************************************/
/*
 parse and print assembly
 */
/**********************************************/
void printAsm (instruction array[], int length)
{
    int i;
    //keep all the ugly formatting characters out of the switch
    fprintf(output,"Line\tOP\tL\tM\t\n");
 
    for (i=0; i< length; i++)
    {
        //print line number
        fprintf(output,"%2d\t\t", i);
 
        //prints out the opcode as a string
        switch (array[i].op)
        {
 
        case (hlt):
            fprintf(output,"hlt");
            break;
 
        case (lit):
            fprintf(output,"lit");
            break;
 
        case (opr):
            fprintf(output,"opr");
            break;
 
        case (lod):
            fprintf(output,"lod");
            break;
 
        case (sto):
            fprintf(output,"sto");
            break;
 
        case (cal):
            fprintf(output,"cal");
            break;
 
        case (inc):
            fprintf(output,"inc");
            break;
 
        case (jmp):
            fprintf(output,"jmp");
            break;
 
        case (jpc):
            fprintf(output,"jpc");
            break;
 
        case (sio):
            fprintf(output,"sio");
            break;
 
        case (10):
            fprintf(output,"sio");
            break;
 
            //something went wrong...
        default:
            fprintf(output,"ERR");
            break;
        }
 
        //keep all the ugly formatting characters out of the switch
        fprintf(output,"\t%2d\t%2d\n", array[i].l, array[i].m);
    }
}
 
/**********************************************/
/*
 print out the first half of the execution print
 */
/**********************************************/
void printInit (instruction instruct)
{
    //print out the line number we are executing
    fprintf(output,"%2d\t\t", PC);
 
    if (printFlag ==1 )
    {
        printf("%2d\t\t", PC);
    }
 
    //print out the instruction's opcode in human readable format
    switch (instruct.op)
    {
    case (hlt):
        fprintf(output,"hlt");
        if (printFlag ==1 )
        {
            printf("hlt");
        }
        break;
 
    case (lit):
        fprintf(output,"lit");
        if (printFlag ==1 )
        {
            printf("lit");
        }
        break;
 
    case (opr):
        fprintf(output,"opr");
        if (printFlag ==1 )
        {
            printf("opr");
        }
        break;
 
    case (lod):
        fprintf(output,"lod");
        if (printFlag ==1 )
        {
            printf("lod");
        }
        break;
 
    case (sto):
        fprintf(output,"sto");
        if (printFlag ==1 )
        {
            printf("sto");
        }
        break;
 
    case (cal):
        fprintf(output,"cal");
        if (printFlag ==1 )
        {
            printf("cal");
        }
        break;
 
    case (inc):
        fprintf(output,"inc");
        if (printFlag ==1 )
        {
            printf("inc");
        }
        break;
 
    case (jmp):
        fprintf(output,"jmp");
        if (printFlag ==1 )
        {
            printf("jmp");
        }
        break;
 
    case (jpc):
        fprintf(output,"jpc");
        if (printFlag ==1 )
        {
            printf("jpc");
        }
        break;
 
    case (sio):
        fprintf(output,"sio");
        if (printFlag ==1 )
        {
            printf("sio");
        }
        break;
 
    case (10):
        fprintf(output,"sio");
        if (printFlag ==1 )
        {
            printf("sio");
        }
 
        break;
 
        //something went wrong...
    default:
        fprintf(output,"ERR");
        if (printFlag ==1 )
        {
            printf("ERR");
        }
        break;
    }
 
    //keep all the ugly formatting characters out of the switch
    fprintf(output,"\t%2d\t%2d\t\t\t", instruct.l, instruct.m);
 
}
 
/**********************************************/
/*
 print out the values of the global pointers
 */
/**********************************************/
void printExec(void)
{
    fprintf(output,"%2d\t%2d\t%2d\t", PC, BP, SP);
 
    if (printFlag == 1)
        printf("%2d\t%2d\t%2d\t", PC, BP, SP);
}
 
/**********************************************/
/*
 print the stack
 */
/**********************************************/
void printStack(void)
{
    //if we have returned to the end of the program, don't print anything
    if (BP == 0)
        return;
 
    //normal case: print out each element of the stack and AR bars
    else
    {
        //uses 1 indexing
        int i;
        for (i=1; i<= SP; i++)
        {
            //this loop prints our activation record bars
            int j;
            for (j=0; j< numARs; j++)
            {
                if (AR[j] == i)
                {
                    fprintf(output,"| ");
                    if (printFlag == 1)
                        printf("| ");
                }
            }
            fprintf(output,"%2d ",stack[i]);
            if (printFlag == 1)
                printf("%2d ",stack[i]);
        }
    }
}
 
/**********************************************/
/*
 This function is used to determine the max
 height of the stack to print.
 */
/**********************************************/
int stackCheck(int arg)
{
    //if SP is bigger than our current counter and we need to print more
    if (SP > maxStack)
        return SP;
    //if SP is smaller than our current counter and we need to print less
    else if (SP < maxStack)
        return SP;
    //if the current counter is fine
    else
        return maxStack;
}
 
//opcode functions
 
/**********************************************/
/*
 OP 1- LIT
 Push constant value (literal) M onto the stack
 */
/**********************************************/
int literal (int L, int M)
{
    SP++;
    stack[SP]=M;
 
    return 1;
}
 
/**********************************************/
/*
 OP 2- OPR
 Operation to be performed on the data at the
 top of the stack
 */
/**********************************************/
int operation(int L, int M)
{
 
    switch(M)
    {
        //RET
    case (RET):
        SP = BP - 1;
        PC = stack[SP + 3];
        BP = stack[SP + 2];
 
        //zero the memory where this AR used to be
        AR[numARs]=0;
        //decrement top counter
        numARs--;
 
        break;
 
        //NEG
    case (NEG):
        stack[SP] = -(stack[SP]);
        break;
 
        //ADD
    case (ADD):
        SP = SP - 1;
        stack[SP] = stack[SP] + stack[SP + 1];
        break;
 
        //SUB
    case (SUB):
        SP = SP - 1;
        stack[SP] = stack[SP] - stack[SP + 1];
        break;
 
        //MUL
    case (MUL):
        SP = SP - 1;
        stack[SP] = stack[SP] * stack[SP + 1];
        break;
 
        //DIV
    case (DIV):
        SP = SP - 1;
        stack[SP] = stack[SP] / stack[SP + 1];
        break;
 
        //ODD
    case (ODD):
        stack[SP] = stack[SP]% 2;
        break;
 
        //MOD
    case (MOD):
        SP = SP - 1;
        stack[SP] = stack[SP] % stack[SP + 1];
        break;
 
        //EQL
    case (EQL):
        SP = SP - 1;
        if(stack[SP] == stack[SP + 1])
        {
            stack[SP] = 1;
        }
        else
            stack[SP] = 0;
        break;
 
        //NEQ
    case (NEQ):
        SP = SP - 1;
        if(stack[SP] != stack[SP + 1])
        {
            stack[SP] = 1;
        }
 
        else
            stack[SP] = 0;
        break;
 
        //LSS
    case (LSS):
        SP = SP - 1;
        if(stack[SP] < stack[SP + 1])
        {
            stack[SP] = 1;
        }
 
        else
            stack[SP] = 0;
        break;
 
        //LEQ
    case (LEQ):
        SP = SP - 1;
        if(stack[SP] <= stack[SP + 1])
        {
            stack[SP] = 1;
        }
 
        else
            stack[SP] = 0;
        break;
 
        //GTR
    case (GTR):
        SP = SP - 1;
        if(stack[SP] > stack[SP + 1])
        {
            stack[SP] = 1;
        }
 
        else
            stack[SP] = 0;
        break;
 
        //GEQ
    case (GEQ):
        SP = SP - 1;
        if(stack[SP] >= stack[SP + 1])
        {
            stack[SP] = 1;
        }
 
        else
            stack[SP] = 0;
        break;
 
        //something went wrong
    default:
        return 0;
 
    }
    return 1;
}
 
/**********************************************/
/*
 OP 3- LOD
 Load value to top of stack from the stack
 location at offset M from L lexicographical
 levels down
 */
/**********************************************/
 
int load (int L, int M)
{
    SP++;
    stack[SP]= stack[base(L, BP) + M];
 
    return 1;
}
 
/**********************************************/
/*
 OP 4- STO
 Store value at top of stack in the stack
 location at offset M from
 L lexicographical levels down
 */
/**********************************************/
int store(int L, int M)
{
    stack[base(L, BP) + M]=stack[SP];
    SP--;
 
    return 1;
}
 
/**********************************************/
/*
 OP 5- CAL
 Call procedure at code index M (generates new
 Activation Record and pc <- M)
 */
/**********************************************/
 
int call(int L, int M)
{
    stack[SP+1]= base(L,BP);    //static link
    stack[SP +2]= BP;           //dynamic link
    stack[SP + 3] = PC;         //return address
    BP= (SP +1);
    PC= M;
 
    //store the base position of the AR at the top index
    AR[numARs]=BP;
    //increment top counter
    numARs++;
 
    if (numARs > MAX_LEXI_LEVELS)
    {
        printf("Error! Nested call overflow, exiting...");
        exit(EXIT_FAILURE);
    }
    return 1;
}
 
/**********************************************/
/*
 OP 6- INC
 Allocate M locals (increment sp by M). First
 three are Static Link (SL), Dynamic Link (DL),
 and Return Address (RA)
 */
/**********************************************/
int increment(int L, int M)
{
 
    SP = SP + M;
 
    return 1;
}
 
/**********************************************/
/*
 OP 7- JMP
 Jump to instruction M
 */
/**********************************************/
int jump(int L, int M)
{
    PC = M;
 
    return 1;
}
 
/**********************************************/
/*
 OP 8- JPC
 Jump to instruction M if top stack element is 0
 */
/**********************************************/
int jumpCondition(int L, int M)
{
    if (stack[SP] ==0)
        PC= M;
    SP--;
 
    return 1;
}
 
/**********************************************/
/*
 OP 9- SIO
 Write the top stack element to the screen

 OP 10- SIO
 Read in input from the user and store it at
 the top of the stack
 */
/**********************************************/
int stdIO(int L, int M)
{
    if (M== 1)
    {
        stdOutReg= stack[SP];
        SP--;
 
        stdOutPrint= TRUE;
 
        return 1;
    }
    else if (M== 2)
    {
        SP++;
        printf("enter integer to be stored to the stack\n");
        scanf("%d",&stack[SP]);
 
        return 1;
    }
 
    return 0;
}
 