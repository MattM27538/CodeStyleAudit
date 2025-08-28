#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "library.h"
#include <assert.h>
#include <stdint.h>

//Consider making this a singleton
struct LineInformation{
    const int8_t maxLineSize;
    int8_t currentLineSize;
    long long lineNumber;
    bool continueReadingFile;
    bool isMultiLineComment;
};

bool readLine(FILE* codeFile, struct LineInformation* lineInformation){
    char lineOfCode[100];
    char charInCode;
    
    //Might be an issue with while != EOF CHECK
    // while((charInCode=fgetc(codeFile)) != EOF){
    for(int i=0; i < lineInformation->maxLineSize; ++i){
        charInCode=fgetc(codeFile);
        
        if(charInCode == EOF){
            lineInformation->continueReadingFile=false;
            //Call function to check final line here.
            return lineInformation->continueReadingFile;
        }
        
        if(charInCode == '\n'){
            break;
        }
        
        lineOfCode[lineInformation->currentLineSize]=charInCode;
        ++lineInformation->currentLineSize;
        
        if(lineInformation->currentLineSize > lineInformation->maxLineSize){
            printf("Line %lld exceeds limit of 100 characters. Correct your code and recompile.", lineInformation->lineNumber);
            exit(1);
        }
        
        // fputc(charInCode, stdout);
        
    }

    printf("%c\n", lineOfCode[0]);
    // if lineOfCode is comment skip. if lineOfCode is multiline keep reading until comment ends.
    if(isComment(lineOfCode, lineInformation->currentLineSize, &lineInformation->isMultiLineComment)
       || lineInformation->isMultiLineComment)
    {   
        // printf("%lld\n", lineInformation->lineNumber);
        //DRY consider creating function for reset. Or add all lines at end of func and return bool var.
        lineInformation->currentLineSize=0;
        ++lineInformation->lineNumber;
        return true;
    }
    else if(isWhileLoop(lineOfCode, lineInformation->currentLineSize)){
        printf("line %lld has while\n", lineInformation->lineNumber);
        //DRY consider creating function for reset. Or add all lines at end of func and return bool var.
        isCorrectWhileLoopFormat(lineOfCode, lineInformation->currentLineSize, lineInformation->lineNumber);
        lineInformation->currentLineSize=0;
        ++lineInformation->lineNumber;
        return true;
    }
    else if(isForLoop(lineOfCode, lineInformation->currentLineSize)){
        printf("line %lld has for\n", lineInformation->lineNumber);
        isCorrectForLoopFormat(lineOfCode, lineInformation->currentLineSize, lineInformation->lineNumber);
        //DRY consider creating function for reset. Or add all lines at end of func and return bool var.
        lineInformation->currentLineSize=0;
        ++lineInformation->lineNumber;
        return true;
    }
    
    // fputc('\n', stdout);
    //DRY consider creating function for reset. Or add all lines at end of func and return bool var.
    lineInformation->currentLineSize=0;
    ++lineInformation->lineNumber;
    return lineInformation->continueReadingFile;
}


int main(int argc, char* argv[]){
    if(!correctCMDLineInput(argc)){
        return EXIT_FAILURE;
    }
    
    FILE* codeFile;
    const char* codeFileName=argv[1];
    const char* readOnly="r";

    struct LineInformation LineInformation={.maxLineSize=100, .currentLineSize=0, 
           .lineNumber=1, .continueReadingFile=true, .isMultiLineComment=false};

    codeFile=fopen(codeFileName, readOnly);
    
    if(codeFile == NULL){
        perror("Error opening code file. Please try again.");
        return EXIT_FAILURE;
    }

    while(readLine(codeFile, &LineInformation));
    
    fclose(codeFile);
    return 0;
}
