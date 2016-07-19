#ifndef _GLODEF_H_

#define MAX_IDENT_NUM 2000

#define IDENTIFIER_MAX_LENGTH   11
#define NUM_MAX_LENGTH          5

#define MAX_SYMBOL_TABLE_SIZE 500
#define MAX_STACK_HEIGHT 2000
#define MAX_CODE_LENGTH 500
#define MAX_LEXI_LEVELS 3

#define MAX_STACK_HEIGHT 2000
#define MAX_CODE_LENGTH 500
#define MAX_LEXI_LEVELS 3


#define _GLODEF_H_



//struct define


//scanner.h

//enum as defined for all symbols
typedef enum
{
    nulsym = 1, 	identsym = 2, 	numbersym = 3, 		plussym = 4,	minussym = 5,
    multsym = 6,  	slashsym = 7, 	oddsym = 8, 		eqlsym = 9, 	neqsym = 10,
    lessym = 11, 	leqsym = 12, 	gtrsym = 13, 		geqsym = 14, 	lparentsym = 15,
    rparentsym = 16,commasym = 17, 	semicolonsym = 18, 	periodsym = 19, becomessym = 20,
    beginsym = 21,  endsym = 22, 	ifsym = 23, 		thensym = 24, 	whilesym = 25,
    dosym = 26,		callsym = 27, 	constsym = 28, 		intsym = 29, 	procsym = 30,
    outsym = 31, 	insym = 32, 	elsesym = 33, 		colonsym = 34,	doublesym=35
} token_type;



//parser.h



typedef struct
{
    int     kind;       // const = 1, var = 2, proc = 3
    int    	name;  			// name up to 11 chars
    int     val;        // number (ASCII value)
    int     level;      // L level
    int     addr;       // M address
} symbol;

//used to store instructions in the internal code array
typedef struct
{
    int     op;
    int     l;
    int     m;
} instruct;






typedef struct
{
    int op; //opcode
    int l;  // L
    int m;  // M
} instruction;




//vm.h

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




//function declaration
int scanner(int printFlag);

int charToTokenType(char symbol);
int tokenizeReservedWord(char* word, int* i);
char* tokenDeparser(int token);



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

int findinsymbol_table(int name);

int vm(int printFlag);

int base            (int l, int base);
void fetch          (instruction i);

//the print functions that the program uses.
//these all output to files.
void printAsm       (instruction array[], int length);
void printInit      (instruction instruct);
void printExec      (void);
void printStack     (void);



//vm.h

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





//identifier declaration and define


FILE* input;
FILE* output;
//FILE* input;
//FILE* output;
FILE* genout;

//FILE* input;
//FILE* output;
int printFlag;
//int printFlag;
//int	printFlag;

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

//int identname[20000];
//int identnamecount=0;

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

//vm.h

//boolean flag to see if we are printing on this line
int stdOutPrint     = FALSE;
//saves value to be printed out for stdIO
int stdOutReg;




int stack           [MAX_STACK_HEIGHT];
int maxStack        = 0;


//the activation record array and top counter variable
int AR[5];
int numARs          = 0;

//Initial stack store
int SP              = 0;
int BP              = 1;
int PC              = 0;
int IR              = 0;


void parsertree(void);

#endif
