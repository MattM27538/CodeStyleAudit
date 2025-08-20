#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "library.h"
#include <assert.h>
#include <stdint.h>

//Consider making this a singleton
struct LineInformation{
    //Consider putting these in a struct.
    const uint8_t maxLineSize;
    uint8_t currentLineSize;
    long long lineNumber;
    bool continueReadingFile;
};

bool readLine(FILE* codeFile, struct LineInformation* LineInformation){
    // char lineOfCode[100];
    char charInCode;
    
    //Might be an issue with while != EOF CHECK
    // while((charInCode=fgetc(codeFile)) != EOF){
    for(int i=0; i < LineInformation->maxLineSize; ++i){
        charInCode=fgetc(codeFile);
        
        if(charInCode == EOF){
            LineInformation->continueReadingFile=false;
            //Call function to check final line here.
            return LineInformation->continueReadingFile;
        }
        
        if(charInCode == '\n'){
            LineInformation->currentLineSize=0;
            ++LineInformation->lineNumber;
            break;
        }
        
        ++LineInformation->currentLineSize;
        
        if(LineInformation->currentLineSize > LineInformation->maxLineSize){
            printf("Line %lld exceeds limit of 100 characters. Correct your code and recompile.", LineInformation->lineNumber);
            exit(1);
        }
        
        // fputc(charInCode, stdout);
        
        // lineOfCode[LineInformation->currentLineSize]=charInCode;
    }
    
    // fputc('\n', stdout);
    LineInformation->currentLineSize=0;
    return LineInformation->continueReadingFile;
}


int main(int argc, char* argv[]){
    if(!correctCMDLineInput(argc)){
        return EXIT_FAILURE;
    }
    
    FILE* codeFile;
    const char* codeFileName=argv[1];
    const char* readOnly="r";

    struct LineInformation LineInformation={.maxLineSize=100, .currentLineSize=0, .lineNumber=1, .continueReadingFile=true};

    codeFile=fopen(codeFileName, readOnly);
    
    if(codeFile == NULL){
        perror("Error opening code file. Please try again.");
        return EXIT_FAILURE;
    }

    while(readLine(codeFile, &LineInformation));
    
    fclose(codeFile);
    return 0;
}
