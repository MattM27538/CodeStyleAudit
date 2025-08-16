#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "library.h"
#include <assert.h>
#include <stdint.h>


int main(int argc, char* argv[]){
    if(!correctCMDLineInput(argc)){
        return EXIT_FAILURE;
    }
    
    FILE* codeFile;
    const char* codeFileName=argv[1];
    const char* readOnly="r";

    codeFile=fopen(codeFileName, readOnly);
    
    if(codeFile == NULL){
        perror("Error opening code file. Please try again.");
        return EXIT_FAILURE;
    }

    char charInCode;
    //Consider putting these in a struct.
    const uint8_t maxLineSize=100;
    uint8_t currentLineSize=0;
    long long lineNumber=1;

    while((charInCode=fgetc(codeFile)) != EOF){
            if(charInCode == '\n'){
                currentLineSize=0;
                ++lineNumber;
            }

            ++currentLineSize;

            if(currentLineSize > maxLineSize){
                printf("Line %lld exceeds limit of 100 characters. Correct your code and recompile.", lineNumber);
                return EXIT_FAILURE;
            }

            // fputc(charInCode, stdout);
    }
    
    fclose(codeFile);
    return 0;
}
