#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "library.h"
#include <assert.h>
#include <stdint.h>

struct LineInformation{
    //Consider putting these in a struct.
    const uint8_t maxLineSize=100;
    uint8_t currentLineSize=0;
    long long lineNumber=1;
};

void readLine(){
    char lineOfCode[100];
    char charInCode;
    
    //Might be an issue with while != EOF CHECK
    while((charInCode=fgetc(codeFile)) != EOF){
        
        if(charInCode == '\n'){
            // lineOfCode[i]='\n';
            currentLineSize=0;
            ++lineNumber;
        }
        
        ++currentLineSize;
        
        if(currentLineSize > maxLineSize){
            printf("Line %lld exceeds limit of 100 characters. Correct your code and recompile.", lineNumber);
            return EXIT_FAILURE;
        }
        
        fputc(charInCode, stdout);
    }  
}


int main(int argc, char* argv[]){
    if(!correctCMDLineInput(argc)){
        return EXIT_FAILURE;
    }
    
    FILE* codeFile;
    const char* codeFileName=argv[1];
    const char* readOnly="r";

    struct LineInformation LineInformation={.maxLineSize=100, .currentLineSize=0, .lineNumber=1};

    codeFile=fopen(codeFileName, readOnly);
    
    if(codeFile == NULL){
        perror("Error opening code file. Please try again.");
        return EXIT_FAILURE;
    }
    
    fclose(codeFile);
    return 0;
}
