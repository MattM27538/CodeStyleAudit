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

bool isKeywordStatement(const char* keywordLiteral, const char* beginningOfLineOfCode){
    return !(strncmp(keywordLiteral, beginningOfLineOfCode, strlen(keywordLiteral)));
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

//TODO Consider combining arguments into struct
//      & split into two functions.
bool checkForParenthesisAndWhiteSpace(const char* lineOfCode, const int charIndex, const long long lineNumber){
    if(lineOfCode[charIndex] == '(' && lineOfCode[charIndex+1] == ' '){
        printf("Error on line %lld: white space found after '('.\n", lineNumber);
        return true;
    }
    else if(lineOfCode[charIndex] == ')' && lineOfCode[charIndex-1] == ' '){
        printf("Error on line %lld: white space found before ')'.\n", lineNumber);
        return true;
    }

    return false;
}

bool isComparisonOperator(const char charInLineOfCode){
    switch(charInLineOfCode){
        case '<':
        case '>':
        case '!':
        case '=':
            {
                return true;
            }
    }

    return false;
}

void auditComparisonOperatorFormat(const char* lineOfCode, const int charIndex, const struct LineInformation* lineInformation){
     switch(lineOfCode[charIndex]){
                case '<':
                case '>':
                case '!':
                    {   
                        if(lineOfCode[charIndex-1] != ' '){
                            printf("Error on line %lld: white space not found before comparison operator.\n", lineInformation->lineNumber);
                        }

                        if((lineOfCode[charIndex+1] == '=' && lineOfCode[charIndex+2] != ' ')
                           || (lineOfCode[charIndex+1] != ' ')){
                            printf("Error on line %lld: white space not found after comparison operator.\n", lineInformation->lineNumber);
                        }
                    }
                break;
                case '=':
                    {
                         if(lineOfCode[charIndex+1] == '=' && lineOfCode[charIndex+2] != ' '){
                            printf("Error on line %lld: white space not found after '=='.\n", lineInformation->lineNumber);
                        }
                        else if(lineOfCode[charIndex-1] == '=' && lineOfCode[charIndex-2] != ' '){
                            printf("Error on line %lld: white space not found before '=='.\n", lineInformation->lineNumber);
                        }
                        //TODO Check for accidental assignment operator
                        // else if (){
                        //     printf("Error on line %lld: assignment operator found.\n", lineNumber);
                        // }
                    }

    }
}

void auditConditionalStatementFormat(const char* lineOfCode, const struct LineInformation* lineInformation){
    for(int charIndex=0; charIndex < lineInformation->currentLineSize; ++charIndex){
        if(checkForParenthesisAndWhiteSpace(lineOfCode, charIndex, lineInformation->lineNumber)){
            continue;
        }
        else if(isComparisonOperator(lineOfCode[charIndex])){
            auditComparisonOperatorFormatting(lineOfCode, charIndex, lineInformation);
        }
    }
}


bool isWhileLoop(const char* lineOfCode, const int lineSize){
    const char* whileStringLiteral="while(";

    char firstSixCharsInLineOfCode[7];

    int firstNonSpaceIndex=findFirstNonSpaceCharInLine(lineOfCode, lineSize);
    
    grabCharsFromString(lineOfCode, firstSixCharsInLineOfCode, sizeof(firstSixCharsInLineOfCode)-1, firstNonSpaceIndex);

    return isKeywordStatement(whileStringLiteral, firstSixCharsInLineOfCode);
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

    char firstFourCharsInLineOfCode[5];

    int firstNonSpaceIndex=findFirstNonSpaceCharInLine(lineOfCode, lineSize);
    
    grabCharsFromString(lineOfCode, firstFourCharsInLineOfCode, sizeof(firstFourCharsInLineOfCode)-1, firstNonSpaceIndex);

    return isKeywordStatement(forStringLiteral, firstFourCharsInLineOfCode);
}

bool isIfStatement(const char* lineOfCode, const int lineSize){
    const char* ifStringLiteral="if(";

    char firstThreeCharsInLineOfCode[4];

    int firstNonSpaceIndex=findFirstNonSpaceCharInLine(lineOfCode, lineSize);
    
    grabCharsFromString(lineOfCode, firstThreeCharsInLineOfCode, sizeof(firstThreeCharsInLineOfCode)-1, firstNonSpaceIndex);

    return isKeywordStatement(ifStringLiteral, firstThreeCharsInLineOfCode);
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

    int firstNonSpaceIndex=findFirstNonSpaceCharInLine(lineOfCode, lineSize);
    
    grabCharsFromString(lineOfCode, firstFourCharsInLineOfCode, sizeof(firstFourCharsInLineOfCode)-1, firstNonSpaceIndex);

    return isKeywordStatement(elseStringLiteral, firstFourCharsInLineOfCode);
}

int findFirstNonSpaceCharInLine(const char* lineOfCode, const int lineSize){
    for(int charIndex=0; charIndex < lineSize; ++charIndex){
        if(lineOfCode[charIndex] != ' '){
            return charIndex;
        }
    }

    return 0;
}

//rename and shrink parameters
void grabCharsFromString(const char* lineOfCode, char* charsInLineOfCode, const int charsInLineOfCodeSize, const int firstNonEmptyindex){
    for(int charIndex=0; charIndex < charsInLineOfCodeSize; ++charIndex){
        charsInLineOfCode[charIndex]=lineOfCode[firstNonEmptyindex+charIndex];
    }

    charsInLineOfCode[charsInLineOfCodeSize]='\0';
}

bool isElseIfStatement(const char* lineOfCode, const int lineSize){
    const char* elseIfStringLiteral="else if";

    char firstSevenCharsInLineOfCode[8];

    //rename var
    int firstNonSpaceIndex=findFirstNonSpaceCharInLine(lineOfCode, lineSize);

    //change 4th parameter name to sizeof()
    grabCharsFromString(lineOfCode, firstSevenCharsInLineOfCode, sizeof(firstSevenCharsInLineOfCode)-1, firstNonSpaceIndex);
        
    return isKeywordStatement(elseIfStringLiteral, firstSevenCharsInLineOfCode);
}

bool isWhiteSpaceAtEndOfLine(const char* lineOfCode, const int8_t currentLineSize){
    //If increment for \n char is added this needs to change to currentLineSize-1
    if((lineOfCode[currentLineSize-2] == ' ') && (currentLineSize != 1)){
        return true;
    }

    return false;
}