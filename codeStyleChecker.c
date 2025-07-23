#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "library.h"
#include <assert.h>


int main(int argc, char* argv[]){
    if(!correctCMDLineInput(argc)){
        return EXIT_FAILURE;
    }
    
    FILE* codeFile;
    codeFile=fopen(argv[1], "r");
    
    if(codeFile == NULL){
        puts("Error opening code file. Please try again.");
        return EXIT_FAILURE;
    }
    
    fclose(codeFile);
    return 0;
}
