#include <stdbool.h>
#include "library.h"
#include <string.h>
#include <stdint.h>

//DELETE ME when done testing.
#include <stdio.h>

bool correctCMDLineInput(int argc){
    if(argc == 2){
        return true;
    }

    //print error/help info
    //printHelp();

    return false;
}

bool isComment(char* lineOfCode, const int lineSize, bool* isMultiLineComment){
    // printf("Line is of size %d\n", lineSize);

    for(int i=0; i < lineSize; ++i){
        if(lineOfCode[i] == ' '){
            continue;
        }
        
        //Confirm '/' can only be used for comments.
        if(lineOfCode[i] == '/'){
            if(lineOfCode[i+1] == '*'){
                *isMultiLineComment=true;
            }

            return true;
        }

        if(lineOfCode[i] == '*' && lineOfCode[i+1] == '/'){
            *isMultiLineComment=false;
            return true;
        }
    }

    return false;
}

void isCorrectWhileFormat(const char* lineOfCode, int8_t currentLineSize, long long lineNumber){
    for(int charInLine=0; charInLine < currentLineSize; ++charInLine){
        if(lineOfCode[charInLine] == '(' && lineOfCode[charInLine+1] == ' '){
            printf("Error on line %lld: white space found after '('.\n", lineNumber);
        }
        else if(lineOfCode[charInLine] == ')' && lineOfCode[charInLine-1] == ' '){
            printf("Error on line %lld: white space found before ')'.\n", lineNumber);
        }
        else{
            switch(lineOfCode[charInLine]){
                case '<':
                case '>':
                case '!':
                    {
                        if(lineOfCode[charInLine-1] != ' '){
                            printf("Error on line %lld: white space not found before comparison operator.\n", lineNumber);
                        }

                        if((lineOfCode[charInLine+1] == '=' && lineOfCode[charInLine+2] != ' ')
                           || (lineOfCode[charInLine+1] != ' ')){
                            printf("Error on line %lld: white space not found after comparison operator.\n", lineNumber);
                        }
                    }
                break;
                case '=':
                    {
                         if(lineOfCode[charInLine+1] == '=' && lineOfCode[charInLine+2] != ' '){
                            printf("Error on line %lld: white space not found after '=='.\n", lineNumber);
                        }
                        else if(lineOfCode[charInLine-1] == '=' && lineOfCode[charInLine-2] != ' '){
                            printf("Error on line %lld: white space not found before '=='.\n", lineNumber);
                        }
                        //Check for accidental assignment operator
                        // else if (){
                        //     printf("Error on line %lld: assignment operator found.\n", lineNumber);
                        // }
                    }

            }
        }
    }
}


bool isWhileLoop(const char* lineOfCode, const int lineSize){
    const char* whileLiteral="while";

    char startOfLineOfCode[6];

    for(int i=0; i < lineSize; ++i){
        if(lineOfCode[i] == ' '){
                continue;
        }

        for(size_t j=0; j<strlen(whileLiteral); ++j){
            if(lineOfCode[i] == '\n'){
                puts("returned false\n");
                return false;
            }
            
            // printf("%zu ", i+j);
            startOfLineOfCode[j]=lineOfCode[i+j];
        }

        startOfLineOfCode[5]='\0';
        // printf("%d\n", strncmp(whileLiteral, placeHolderName, strlen(whileLiteral)));
        // printf("%s =? ", placeHolderName);
        // printf("%s\n", whileLiteral);
        return !(strncmp(whileLiteral, startOfLineOfCode, strlen(whileLiteral)));
    }

    return false;
}

bool isForLoop(const char* lineOfCode, const int lineSize){
    const char* forLiteral="for";

    char startOfLineOfCode[4];

    for(int i=0; i < lineSize; ++i){
        if(lineOfCode[i] == ' '){
                continue;
        }

        for(size_t j=0; j<strlen(forLiteral); ++j){
            if(lineOfCode[i] == '\n'){
                puts("returned false\n");
                return false;
            }
            
            // printf("%zu ", i+j);
            startOfLineOfCode[j]=lineOfCode[i+j];
        }

        startOfLineOfCode[3]='\0';
        // printf("%d\n", strncmp(whileLiteral, placeHolderName, strlen(whileLiteral)));
        // printf("%s =? ", placeHolderName);
        // printf("%s\n", whileLiteral);
        return !(strncmp(forLiteral, startOfLineOfCode, strlen(forLiteral)));
    }

    return false;
}