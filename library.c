#include <stdbool.h>
#include "library.h"
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

//TODO:
//CHECK FIRST CHAR OF LINE THEN CALL APPROPRIATE FUNCTION
//CHECK FOR MULTILINE COMPARISONS
//add check for assignment operator format
//Add isassignmentoperator/audit assignmentoperator?
//Has second assignment operator for loop?
//Breakup Readline
//move main into a function

bool correctCMDLineInput(int argc){
    if(argc == 2){
        return true;
    }

    fprintf(stderr, "Incorrect number of command line arguments found. Expected 2, found %d.\nExiting.", argc);

    return false;
}

FILE* openCodeFile(struct CodeFile* codeFile){
    FILE* codeFilePointer = fopen(codeFile->codeFileName, codeFile->readOnly);
    
    if(codeFilePointer == NULL){
        perror("Error opening code file. Please try again.");
        exit(1);
    }

    return codeFilePointer;
}

bool readLine(FILE* codeFile, struct LineOfCode* lineOfCode){
    char charInCode;
    
    //Might be an issue with while != EOF CHECK
    // while((charInCode=fgetc(codeFile)) != EOF){
    for(int i = 0; i < lineOfCode->maxLineSize; ++i){
        charInCode = fgetc(codeFile);
        
        if(charInCode == EOF){
            lineOfCode->continueReadingFile = false;
            //Call function to check final line here.
            return lineOfCode->continueReadingFile;
        }
        
        //Entire Program is off by 1 index because of not incrementing currentLineSize here
        //Decide if you want to change this.
        if(charInCode == '\n'){
            break;
        }
        
        lineOfCode->codeText[lineOfCode->lineSize] = charInCode;
        ++lineOfCode->lineSize;
        
        if(lineOfCode->lineSize > lineOfCode->maxLineSize){
            printf("Line %lld exceeds limit of 100 characters. Correct your code and recompile.", lineOfCode->lineNumber);
            exit(1);
        }
        
        // fputc(charInCode, stdout);
        
    }

    lineOfCode->firstCharInLine = getFirstCharInLine(lineOfCode);

    matchFirstCharInLineToInstruction(lineOfCode);

    if(isWhiteSpaceAtEndOfLine(lineOfCode)){
        printf("Syntax Error: Found trailing whitespace on line %lld\n", lineOfCode->lineNumber);
    }

    //DRY consider creating function for reset. Or add all lines at end of func and return bool var.
    lineOfCode->lineSize = 0;
    ++lineOfCode->lineNumber;
    return lineOfCode->continueReadingFile;
}

char getFirstCharInLine(const struct LineOfCode* lineOfCode){
    for(int charIndex = 0; charIndex < lineOfCode->lineSize; ++charIndex){
        if(lineOfCode->codeText[charIndex] != ' '){
            return lineOfCode->codeText[charIndex];
        }
    }
    //This will probably break something.
    return '\n';
}

bool isKeywordStatement(const char* keywordLiteral, const char* beginningOfLineOfCode){
    return !(strncmp(keywordLiteral, beginningOfLineOfCode, strlen(keywordLiteral)));
}

//lineOfCode->codeText and firstCharInLine add to struct?
void matchFirstCharInLineToInstruction(struct LineOfCode* lineOfCode){
    switch(lineOfCode->firstCharInLine){
        case '/':
        {   
            //Pass ineInformation instead of last two parameters.
            if(isComment(lineOfCode)
            || lineOfCode->isMultiLineComment){

            }
        }
        break;
        case 'w':
        {
            if(isWhileLoop(lineOfCode)){
                printf("line %lld has while\n", lineOfCode->lineNumber);
                auditConditionalStatementFormat(lineOfCode);
            }
        }
        break;
        case 'f':
        {
            if(isForLoop(lineOfCode)){
                printf("line %lld has for\n", lineOfCode->lineNumber);
                isCorrectForLoopFormat(lineOfCode);
            }
        }
        break;
        case 'i':
        {
            if(isIfStatement(lineOfCode)){
                printf("line %lld has if statement\n", lineOfCode->lineNumber);
                auditConditionalStatementFormat(lineOfCode);
            }
        }
        break;
        case 'e':
        {
            if(isElseStatement(lineOfCode)){
                if(isElseIfStatement(lineOfCode)){
                    printf("line %lld has else if statement\n", lineOfCode->lineNumber);
                    auditConditionalStatementFormat(lineOfCode);
                }
                else{
                    printf("line %lld has else statement\n", lineOfCode->lineNumber);
                    isCorrectElseStatementFormat(lineOfCode);
                }
            }
        }
        break;
    }
}

bool isComment(struct LineOfCode* lineOfCode){
    for(int charIndex = 0; charIndex < lineOfCode->lineSize; ++charIndex){
        if(lineOfCode->codeText[charIndex] == ' '){
            continue;
        }

        if(lineOfCode->codeText[charIndex] == '/'){
            if(lineOfCode->codeText[charIndex + 1] == '/'){         
                return true;
            } 
            else if(isStartOfMultiLineComment(lineOfCode, charIndex)){
                setMultiLineCommentFlag(lineOfCode);
                return true;
            }
            else if(isEndOfMultiLineComment(lineOfCode, charIndex)){
                resetMultiLineCommentFlag(lineOfCode);
                return true;
            }
        }
    }

    return false;
}

bool isStartOfMultiLineComment(const struct LineOfCode* lineOfCode, const int charIndex){
    if(lineOfCode->codeText[charIndex + 1] == '*' ){
        return true;
    }

    return false;
}

void setMultiLineCommentFlag(struct LineOfCode* lineOfCode){
    lineOfCode->isMultiLineComment = true;
}

bool isEndOfMultiLineComment(const struct LineOfCode* lineOfCode, const int charIndex){
    if(lineOfCode->codeText[charIndex - 1] == '*'){
        return true;
    }

    return false;
}

void resetMultiLineCommentFlag(struct LineOfCode* lineOfCode){
    lineOfCode->isMultiLineComment = false;
}

bool isParenthesis(const char charInLineOfCode){
    if((charInLineOfCode == ')') || (charInLineOfCode == '(')){
        return true;
    }

    return false;
}

void auditParenthesisFormat(const struct LineOfCode* lineOfCode, const int charIndex){
    if(((lineOfCode->codeText[charIndex] == '(') && (lineOfCode->codeText[charIndex+1] == ' '))){
        printf("Error on line %lld: white space found after '('.\n", lineOfCode->lineNumber);
    }
    else if((lineOfCode->codeText[charIndex] == ')') && (lineOfCode->codeText[charIndex-1] == ' ')){
        printf("Error on line %lld: white space found before ')'.\n", lineOfCode->lineNumber);
    }
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

void auditComparisonOperatorFormat(const struct LineOfCode* lineOfCode, const int charIndex){
     switch(lineOfCode->codeText[charIndex]){
                case '<':
                case '>':
                case '!':
                    {   
                        if(lineOfCode->codeText[charIndex-1] != ' '){
                            printf("Error on line %lld: white space not found before comparison operator.\n", lineOfCode->lineNumber);
                        }

                        if((lineOfCode->codeText[charIndex+1] == '=' && lineOfCode->codeText[charIndex+2] != ' ')
                           || (lineOfCode->codeText[charIndex+1] != ' ')){
                            printf("Error on line %lld: white space not found after comparison operator.\n", lineOfCode->lineNumber);
                        }
                    }
                break;
                case '=':
                    {
                         if(lineOfCode->codeText[charIndex+1] == '=' && lineOfCode->codeText[charIndex+2] != ' '){
                            printf("Error on line %lld: white space not found after '=='.\n", lineOfCode->lineNumber);
                        }
                        else if(lineOfCode->codeText[charIndex-1] == '=' && lineOfCode->codeText[charIndex-2] != ' '){
                            printf("Error on line %lld: white space not found before '=='.\n", lineOfCode->lineNumber);
                        }
                        //TODO Check for accidental assignment operator
                        // else if (){
                        //     printf("Error on line %lld: assignment operator found.\n", lineNumber);
                        // }
                    }

    }
}

void auditConditionalStatementFormat(const struct LineOfCode* lineOfCode){
    for(int charIndex = 0; charIndex < lineOfCode->lineSize; ++charIndex){
        // if(checkForParenthesisAndWhiteSpace(lineOfCode->codeText, charIndex, lineOfCode->lineNumber)){
        if(isParenthesis(lineOfCode->codeText[charIndex])){
            auditParenthesisFormat(lineOfCode, charIndex);
        }
        else if(isComparisonOperator(lineOfCode->codeText[charIndex])){
            auditComparisonOperatorFormat(lineOfCode, charIndex);
        }
    }
}


bool isWhileLoop(const struct LineOfCode* lineOfCode){
    const char* whileStringLiteral = "while(";

    char firstSixCharsInLineOfCode[7];

    int firstNonSpaceIndex = findFirstNonSpaceCharInLine(lineOfCode);
    
    grabCharsFromString(lineOfCode, firstSixCharsInLineOfCode, sizeof(firstSixCharsInLineOfCode)-1, firstNonSpaceIndex);

    return isKeywordStatement(whileStringLiteral, firstSixCharsInLineOfCode);
}

void isCorrectForLoopFormat(const struct LineOfCode* lineOfCode){
    for(int charIndex = 0; charIndex < lineOfCode->lineSize; ++charIndex){

        if(isParenthesis(lineOfCode->codeText[charIndex])){
            auditParenthesisFormat(lineOfCode, charIndex);
        }
        else if(isSemiColon(lineOfCode->codeText[charIndex])){
            auditSemiColonFormat(lineOfCode, charIndex);
        }
        else if(isComparisonOperator(lineOfCode->codeText[charIndex])){
            auditComparisonOperatorFormat(lineOfCode, charIndex);
        }
    }
}

bool isSemiColon(const char charInLineOfCode){
    if(charInLineOfCode == ';'){
        return true;
    }

    return false;
}

void auditSemiColonFormat(const struct LineOfCode* lineOfCode, const int charIndex){
    if(lineOfCode->codeText[charIndex-1] == ' '){
        printf("Error on line %lld: white space found before ';' on column %d.\n", lineOfCode->lineNumber, charIndex+1);
    }
    else if(lineOfCode->codeText[charIndex+1] != ' '){
        printf("Error on line %lld: white space not found after ';' on column %d.\n", lineOfCode->lineNumber, charIndex+1);
    }
}

//TODO replace all lineSize with struct member?
bool isForLoop( const struct LineOfCode* lineOfCode){
    const char* forStringLiteral = "for(";
    
    char firstFourCharsInLineOfCode[5];
    
    int firstNonSpaceIndex = findFirstNonSpaceCharInLine(lineOfCode);
    
    grabCharsFromString(lineOfCode, firstFourCharsInLineOfCode, sizeof(firstFourCharsInLineOfCode)-1, firstNonSpaceIndex);
    
    return isKeywordStatement(forStringLiteral, firstFourCharsInLineOfCode);
}

bool isIfStatement(const struct LineOfCode* lineOfCode){
    const char* ifStringLiteral = "if(";

    char firstThreeCharsInLineOfCode[4];

    int firstNonSpaceIndex = findFirstNonSpaceCharInLine(lineOfCode);
    
    grabCharsFromString(lineOfCode, firstThreeCharsInLineOfCode, sizeof(firstThreeCharsInLineOfCode)-1, firstNonSpaceIndex);

    return isKeywordStatement(ifStringLiteral, firstThreeCharsInLineOfCode);
}

void isCorrectElseStatementFormat(const struct LineOfCode* lineOfCode){
    for(int charInLine = 0; charInLine < lineOfCode->lineSize; ++charInLine){
        if(lineOfCode->codeText[charInLine] == '{'){
            if(lineOfCode->codeText[charInLine-1] != 'e'){
                printf("Syntax Error: '{' not found directly after end of else"
                       " statement on line %lld.\n", lineOfCode->lineNumber);
            }
            return;
        }
    }

    puts("Syntax Error: \"{\" not found on same line as else statement.\n");
}

bool isElseStatement(const struct LineOfCode* lineOfCode){
    const char* elseStringLiteral = "else";

    char firstFourCharsInLineOfCode[5];

    int firstNonSpaceIndex = findFirstNonSpaceCharInLine(lineOfCode);
    
    grabCharsFromString(lineOfCode, firstFourCharsInLineOfCode, sizeof(firstFourCharsInLineOfCode)-1, firstNonSpaceIndex);

    return isKeywordStatement(elseStringLiteral, firstFourCharsInLineOfCode);
}

int findFirstNonSpaceCharInLine(const struct LineOfCode* lineOfCode){
    for(int charIndex = 0; charIndex < lineOfCode->lineSize; ++charIndex){
        if(lineOfCode->codeText[charIndex] != ' '){
            return charIndex;
        }
    }

    return 0;
}

//rename and shrink parameters
void grabCharsFromString(const struct LineOfCode* lineOfCode, char* charsInLineOfCode, const int charsInLineOfCodeSize, const int firstNonEmptyindex){
    for(int charIndex = 0; charIndex < charsInLineOfCodeSize; ++charIndex){
        charsInLineOfCode[charIndex] = lineOfCode->codeText[firstNonEmptyindex+charIndex];
    }

    charsInLineOfCode[charsInLineOfCodeSize] = '\0';
}

bool isElseIfStatement(const struct LineOfCode* lineOfCode){
    const char* elseIfStringLiteral = "else if";

    char firstSevenCharsInLineOfCode[8];

    //rename var
    int firstNonSpaceIndex = findFirstNonSpaceCharInLine(lineOfCode);

    //change 4th parameter name to sizeof()
    grabCharsFromString(lineOfCode, firstSevenCharsInLineOfCode, sizeof(firstSevenCharsInLineOfCode)-1, firstNonSpaceIndex);
        
    return isKeywordStatement(elseIfStringLiteral, firstSevenCharsInLineOfCode);
}

bool isWhiteSpaceAtEndOfLine(const struct LineOfCode* lineOfCode){
    //If increment for \n char is added this needs to change to currentLineSize-1
    if((lineOfCode->codeText[lineOfCode->lineSize-2] == ' ') && (lineOfCode->lineSize != 1)){
        return true;
    }

    return false;
}