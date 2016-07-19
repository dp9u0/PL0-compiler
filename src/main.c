/**********************************************************
 *file name :main.c
 *describe : do parser transcode works
 *copyright :written by dempkwok
 *time : 2013.6.10
 **********************************************************
 */


#include <stdio.h>
#include "vm.h"
#include "scanner.h"
#include "parser.h"
#include "glodef.h"

int main(int argc, char *argv[])
{
    int lexemeFlag=0;
    int genFlag=0;
    int traceFlag=0;
    int sFlag=1;
    int pFlag=1;
    
    int i;
    for (i=0; i< argc; i++)
    {
        if (strcmp(argv[i], "-l") ==0)
            lexemeFlag=1;

        if (strcmp(argv[i], "-a") ==0)
            genFlag=1;

        if (strcmp(argv[i], "-v") ==0)
            traceFlag=1;
            
    }
    printf("\n------SCANNER RUNNING------\n");
    sFlag=scanner(lexemeFlag);
    
    printf("\n------PARSER SEMAER CREATE OBJECT CODING ------\n");
    
    pFlag=parser(genFlag);
    
    printf("\n------COMPILER FINISH------\n");

    if(sFlag==0 && pFlag==0)
    {
	    printf("\n------RUNNING CODE IN VM------\n");
	    vm(traceFlag);
    }
    printf("\n------ALL WORK FINISH------\n");
    /*   -l : print the list of lexemes/tokens (scanner output) to the screen
        ​ -a : print the generated assembly code (parser/codegen output) to the screen
        ​ -v : print virtual machine execution trace (virtual machine output) to the screen */
}
