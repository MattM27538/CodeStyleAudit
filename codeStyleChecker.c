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

///Final version
// bool isComment(char* lineOfCode, Lineinformation* Lineinformation){
//     for(int i=0; i < Lineinformation->currentLineSize; ++i){
//         if(lineOfCode[i] == ' '){
//             continue;
//         }

//         if(lineOfCode[i] == '/'){
//             if(lineOfCode[i+1] == '/'){

//             }
//             else if(lineOfCode[i+1] == '*'){
//                 Lineinformation.isMultiLineComment=true;
//             }

//             return true;
//         }

//         return false;
//     }
// }



bool readLine(FILE* codeFile, struct LineInformation* LineInformation){
    char lineOfCode[100];
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
        
        lineOfCode[LineInformation->currentLineSize]=charInCode;
    }

    // if lineOfCode is comment skip. if lineOfCode is multiline keep reading until comment ends.
    if(isComment(lineOfCode, lineInformation->currentLineSize, lineInformation->isMultiLineComment)
       || lineInformation->isMultiLineComment)
    {
        return true;
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
