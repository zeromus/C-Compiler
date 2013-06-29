/***************************
C-minus Compiler -
Compiler Design Course Project
***************************/

#include "globals.h"
#include "utils.h"
#include "parse.h"
#include "symtab.h"
#include "codegen.h"

#define FILE_NAME_LEN 100

FILE* source; /* source code text file */
FILE* listing; /* listing output text file */
FILE* code; /* code text file for TM simulator */


/*debug flags*/
int EchoSource = FALSE;
int TraceScan = FALSE;
int TraceParse = FALSE;
int TraceAnalyze = FALSE;
int TraceCode = TRUE;

int Error = FALSE;



int main(int argc, char *argv[])
{
  
    char sourcefile[FILE_NAME_LEN]; /* source code file name */

    if (argc != 2){ 
        fprintf(stderr,"usage: %s <filename>\n",argv[0]);
        strcpy(sourcefile,"./test/test0.cm") ;
    }
    else{
        strcpy(sourcefile,argv[1]) ;
    }

    source = fopen(sourcefile,"r");
    ASSERT(source != NULL){ 
      fprintf(stderr,"File %s not found.\n",sourcefile);
    }

    listing = stdout; /* send listing to screen */
    fprintf(listing,"\nC-minus Compiler\ntarget: %s\n",sourcefile);

    initTable();
    yyrestart(source);
    yyparse();
    fclose(source);
    

    if (! Error){ 
      fprintf(listing,"\nParsing Finished...\n");
      fprintf(listing,"\nSemantic Analysis Finished...\n");
    }
    else{
      fprintf(listing, "\nError occurred in Parsing&Analyzing!\n");
      return 1;
    }


    char * codefile = (char *) calloc(strlen(sourcefile), sizeof(char));
    strcpy(codefile,sourcefile);
    strcat(codefile,".tm");
    code = fopen(codefile,"w");
    ASSERT(code != NULL){ 
      fprintf(stderr, "Unable to open %s for output.\n",codefile);
    }
    codeGen();
    fclose(code);

    if(! Error){
      fprintf(listing,"\nCode Generation Finished...\n");
      fprintf(listing, "\nSee %s for result codes.\n", codefile);
    }
    else{
      fprintf(listing, "\nError occurred in Code generating!\n");
      return 1;
    }
    

    return 0;
}


