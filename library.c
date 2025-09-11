#include <stdbool.h>
#include "library.h"
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

//TODO:
//CHECK FIRST CHAR OF LINE THEN CALL APPROPRIATE FUNCTION
//CHECK FOR MULTILINE COMPARISONS

bool correctCMDLineInput(int argc){
    if(argc == 2){
        return true;
    }

    fprintf(stderr, "Incorrect number of command line arguments found. Expected 2, found %d.\nExiting.", argc);

    return false;
}

FILE* openCodeFile(struct CodeFile* codeFile){
    FILE* codeFilePointer=fopen(codeFile->codeFileName, codeFile->readOnly);
    
    if(codeFilePointer == NULL){
        perror("Error opening code file. Please try again.");
        exit(1);
    }

    return codeFilePointer;
}

char getFirstCharInLine(const char* lineOfCode, const int lineSize){
    for(int charIndex=0; charIndex < lineSize; ++charIndex){
        if(lineOfCode[charIndex] != ' '){
            return lineOfCode[charIndex];
        }
    }
    //This will probably break something.
    return '\n';
}

//lineOfCode and firstCharInLine add to struct?
void matchFirstCharInLineToInstruction(const char* lineOfCode, struct LineInformation* lineInformation){
    switch(lineInformation->firstCharInLine){
        case '/':
        {   
            //Pass ineInformation instead of last two parameters.
            if(isComment(lineOfCode, lineInformation->currentLineSize, &lineInformation->isMultiLineComment)
            || lineInformation->isMultiLineComment){

            }
        }
        break;
        case 'w':
        {
            if(isWhileLoop(lineOfCode, lineInformation->currentLineSize)){
                printf("line %lld has while\n", lineInformation->lineNumber);
                auditConditionalStatementFormat(lineOfCode, lineInformation);
            }
        }
        break;
        case 'f':
        {
            if(isForLoop(lineOfCode, lineInformation->currentLineSize)){
                printf("line %lld has for\n", lineInformation->lineNumber);
                isCorrectForLoopFormat(lineOfCode, lineInformation);
            }
        }
        break;
        case 'i':
        {
            if(isIfStatement(lineOfCode, lineInformation->currentLineSize)){
                printf("line %lld has if statement\n", lineInformation->lineNumber);
                auditConditionalStatementFormat(lineOfCode, lineInformation);
            }
        }
        break;
        case 'e':
        {
            if(isElseStatement(lineOfCode, lineInformation->currentLineSize)){
                if(isElseIfStatement(lineOfCode, lineInformation->currentLineSize)){
                    printf("line %lld has else if statement\n", lineInformation->lineNumber);
                    auditConditionalStatementFormat(lineOfCode, lineInformation);
                }
                else{
                    printf("line %lld has else statement\n", lineInformation->lineNumber);
                    isCorrectElseStatementFormat(lineOfCode, lineInformation);
                }
            }
        }
        break;
        // default:{
        //     puts("Did default");
        // }
        //call is function call
    }
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

void auditConditionalStatementFormat(const char* lineOfCode, const struct LineInformation* lineInformation){
    for(int charInLine=0; charInLine < lineInformation->currentLineSize; ++charInLine){
        // if(checkForParenthesisAndWhiteSpace(lineOfCode, charInLine)){
        //     continue;
        // };
        if(lineOfCode[charInLine] == '(' && lineOfCode[charInLine+1] == ' '){
            printf("Error on line %lld: white space found after '('.\n", lineInformation->lineNumber);
        }
        else if(lineOfCode[charInLine] == ')' && lineOfCode[charInLine-1] == ' '){
            printf("Error on line %lld: white space found before ')'.\n", lineInformation->lineNumber);
        }
        else{
            switch(lineOfCode[charInLine]){
                case '<':
                case '>':
                case '!':
                    {
                        if(lineOfCode[charInLine-1] != ' '){
                            printf("Error on line %lld: white space not found before comparison operator.\n", lineInformation->lineNumber);
                        }

                        if((lineOfCode[charInLine+1] == '=' && lineOfCode[charInLine+2] != ' ')
                           || (lineOfCode[charInLine+1] != ' ')){
                            printf("Error on line %lld: white space not found after comparison operator.\n", lineInformation->lineNumber);
                        }
                    }
                break;
                case '=':
                    {
                         if(lineOfCode[charInLine+1] == '=' && lineOfCode[charInLine+2] != ' '){
                            printf("Error on line %lld: white space not found after '=='.\n", lineInformation->lineNumber);
                        }
                        else if(lineOfCode[charInLine-1] == '=' && lineOfCode[charInLine-2] != ' '){
                            printf("Error on line %lld: white space not found before '=='.\n", lineInformation->lineNumber);
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
    const char* whileStringLiteral="while(";

    char startOfLineOfCode[7];

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

        startOfLineOfCode[6]='\0';

        return !(strncmp(whileStringLiteral, startOfLineOfCode, strlen(whileStringLiteral)));
    }

    return false;
}

void isCorrectForLoopFormat(const char* lineOfCode, const struct LineInformation* lineInformation){
    for(int charInLine=0; charInLine < lineInformation->currentLineSize; ++charInLine){
        if(lineOfCode[charInLine] == '(' && lineOfCode[charInLine+1] == ' '){
            printf("Error on line %lld: white space found after '(' on column %d.\n", lineInformation->lineNumber, charInLine+1);
        }
        else if(lineOfCode[charInLine] == ')' && lineOfCode[charInLine-1] == ' '){
            printf("Error on line %lld: white space found before ')' on column %d.\n", lineInformation->lineNumber, charInLine+1);
        }
        else if(lineOfCode[charInLine] == ';' && lineOfCode[charInLine-1] == ' '){
            printf("Error on line %lld: white space found before ';' on column %d.\n", lineInformation->lineNumber, charInLine+1);
        }
        else if(lineOfCode[charInLine] == ';' && lineOfCode[charInLine+1] != ' '){
            printf("Error on line %lld: white space not found after ';' on column %d.\n", lineInformation->lineNumber, charInLine+1);
        }
    }

    //Add switch statement/switch func call here
}

bool isForLoop(const char* lineOfCode, const int lineSize){
    const char* forStringLiteral="for(";

    char startOfLineOfCode[5];

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

        startOfLineOfCode[4]='\0';
        
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

void isCorrectElseStatementFormat(const char* lineOfCode, const struct LineInformation* lineInformation){
    for(int charInLine=0; charInLine < lineInformation->currentLineSize; ++charInLine){
        if(lineOfCode[charInLine] == '{'){
            if(lineOfCode[charInLine-1] != 'e'){
                printf("Syntax Error: '{' not found directly after end of else"
                       " statement on line %lld.\n", lineInformation->lineNumber);
            }
            return;
        }
    }

    puts("Syntax Error: \"{\" not found on same line as else statement.\n");
}

bool isElseStatement(const char* lineOfCode, const int lineSize){
    const char* elseStringLiteral="else";

    char firstFourCharsInLineOfCode[5];

    for(int i=0; i < lineSize; ++i){
        if(lineOfCode[i] == ' '){
            continue;
        }

        for(size_t j=0; j<strlen(elseStringLiteral); ++j){
            if(lineOfCode[i] == '\n'){
                puts("returned false\n");
                return false;
            }

            firstFourCharsInLineOfCode[j]=lineOfCode[i+j];
        }

        firstFourCharsInLineOfCode[4]='\0';
        
        return !(strncmp(elseStringLiteral, firstFourCharsInLineOfCode, strlen(elseStringLiteral)));
    }

    return false;
}

int findFirstNonSpaceCharInLine(const char* lineOfCode, const int lineSize){
    for(int charIndex=0; charIndex < lineSize; ++charIndex){
        if(lineOfCode[charIndex] == ' '){
            continue;
        }

        return charIndex;
    }

    return 0;
}

//rename and shrink parameters
void grabCharsFromString(const char* lineOfCode, char* charsInLineOfCode, const int charsInLineOfCodeSize, const int firstNonEmptyindex){
    for(int charIndex=firstNonEmptyindex; charIndex < charsInLineOfCodeSize; ++charIndex){
        charsInLineOfCode[charIndex]=lineOfCode[firstNonEmptyindex+charIndex];
    }

    charsInLineOfCode[charsInLineOfCodeSize]='\0';
}

bool isElseIfStatement(const char* lineOfCode, const int lineSize){
    const char* elseIfStringLiteral="else if";

    char firstSevenCharsInLineOfCode[8];

    //rename var
    int firstNonSpaceindex=findFirstNonSpaceCharInLine(lineOfCode, lineSize);

    //change 4th parameter name to sizeof()
    grabCharsFromString(lineOfCode, firstSevenCharsInLineOfCode, sizeof(firstSevenCharsInLineOfCode)-1, firstNonSpaceindex);

    // for(int i=0; i < lineSize; ++i){
    //     if(lineOfCode[i] == ' '){
    //         continue;
    //     }

        // for(size_t j=0; j<strlen(elseIfStringLiteral); ++j){
        //     if(lineOfCode[i] == '\n'){
        //         puts("returned false\n");
        //         return false;
        //     }

        //     firstSevenCharsInLineOfCode[j]=lineOfCode[i+j];
        // }

        //move to grabCharsFromString
        // firstSevenCharsInLineOfCode[7]='\0';
        
        return !(strncmp(elseIfStringLiteral, firstSevenCharsInLineOfCode, strlen(elseIfStringLiteral)));
    // }

    return false;
}

bool isWhiteSpaceAtEndOfLine(const char* lineOfCode, const int8_t currentLineSize){
    //If increment for \n char is added this needs to change to currentLineSize-1
    if((lineOfCode[currentLineSize-2] == ' ') && (currentLineSize != 1)){
        return true;
    }

    return false;
}