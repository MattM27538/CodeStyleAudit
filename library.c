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

bool isComment(const char* lineOfCode, const int lineSize, bool* isMultiLineComment){
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

bool checkForParenthesisAndWhiteSpace(const char* lineOfCode, const int charInLine){
    if(lineOfCode[charInLine] == '(' && lineOfCode[charInLine+1] == ' '){
    //     printf("Error on line %lld: white space found after '('.\n", lineNumber);
    //     return true;
    }
    else if(lineOfCode[charInLine] == ')' && lineOfCode[charInLine-1] == ' '){
    //     printf("Error on line %lld: white space found before ')'.\n", lineNumber);
    //     return true;
    }

    return false;
}

void isCorrectWhileLoopFormat(const char* lineOfCode, const int8_t currentLineSize, const long long lineNumber){
    for(int charInLine=0; charInLine < currentLineSize; ++charInLine){
        // if(checkForParenthesisAndWhiteSpace(lineOfCode, charInLine)){
        //     continue;
        // };
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
    const char* whileStringLiteral="while";

    char startOfLineOfCode[6];

    for(int i=0; i < lineSize; ++i){
        if(lineOfCode[i] == ' '){
            continue;
        }

        for(size_t j=0; j<strlen(whileStringLiteral); ++j){
            if(lineOfCode[i] == '\n'){
                puts("returned false\n");
                return false;
            }
            
            startOfLineOfCode[j]=lineOfCode[i+j];
        }

        startOfLineOfCode[5]='\0';

        return !(strncmp(whileStringLiteral, startOfLineOfCode, strlen(whileStringLiteral)));
    }

    return false;
}

void isCorrectForLoopFormat(const char* lineOfCode, const int8_t currentLineSize, const long long lineNumber){
    for(int charInLine=0; charInLine < currentLineSize; ++charInLine){
        if(lineOfCode[charInLine] == '(' && lineOfCode[charInLine+1] == ' '){
            printf("Error on line %lld: white space found after '(' on column %d.\n", lineNumber, charInLine+1);
        }
        else if(lineOfCode[charInLine] == ')' && lineOfCode[charInLine-1] == ' '){
            printf("Error on line %lld: white space found before ')' on column %d.\n", lineNumber, charInLine+1);
        }
        else if(lineOfCode[charInLine] == ';' && lineOfCode[charInLine-1] == ' '){
            printf("Error on line %lld: white space found before ';' on column %d.\n", lineNumber, charInLine+1);
        }
        else if(lineOfCode[charInLine] == ';' && lineOfCode[charInLine+1] != ' '){
            printf("Error on line %lld: white space not found after ';' on column %d.\n", lineNumber, charInLine+1);
        }
    }
}

bool isForLoop(const char* lineOfCode, const int lineSize){
    const char* forStringLiteral="for";

    char startOfLineOfCode[4];

    for(int i=0; i < lineSize; ++i){
        if(lineOfCode[i] == ' '){
            continue;
        }

        for(size_t j=0; j<strlen(forStringLiteral); ++j){
            if(lineOfCode[i] == '\n'){
                puts("returned false\n");
                return false;
            }

            startOfLineOfCode[j]=lineOfCode[i+j];
        }

        startOfLineOfCode[3]='\0';
        
        return !(strncmp(forStringLiteral, startOfLineOfCode, strlen(forStringLiteral)));
    }

    return false;
}

bool isIfStatement(const char* lineOfCode, const int lineSize){
    const char* ifStringLiteral="if(";

    char firstThreeCharsInLineOfCode[4];

    for(int i=0; i < lineSize; ++i){
        if(lineOfCode[i] == ' '){
            continue;
        }

        for(size_t j=0; j<strlen(ifStringLiteral); ++j){
            if(lineOfCode[i] == '\n'){
                puts("returned false\n");
                return false;
            }

            firstThreeCharsInLineOfCode[j]=lineOfCode[i+j];
        }

        firstThreeCharsInLineOfCode[3]='\0';
        
        return !(strncmp(ifStringLiteral, firstThreeCharsInLineOfCode, strlen(ifStringLiteral)));
    }

    return false;
}

bool isWhiteSpaceAtEndOfLine(const char* lineOfCode, const int8_t currentLineSize){
    //If increment for \n char is added this needs to change to currentLineSize-1
    if((lineOfCode[currentLineSize-2] == ' ') && (currentLineSize != 1)){
        return true;
    }

    return false;
}