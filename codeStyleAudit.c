#include <stdbool.h>
#include "codeStyleAudit.h"
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
//account for being function names starting with else, while, if. if rest of line is empty print missing ( or {, else print warning to not use as non keyword statement.
//account for spaced out keyword statements i.e. if    (){}.
//change func name to grabNcharsFromSTring

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
    
    //TODO Might be an issue with while != EOF CHECK
    // while((charInCode=fgetc(codeFile)) != EOF){
    for(int i = 0; i < lineOfCode->maxLineSize + 1; ++i){
        charInCode = fgetc(codeFile);
        
        if(charInCode == EOF){
            lineOfCode->continueReadingFile = false;
            //Call function to check final line here.
            return lineOfCode->continueReadingFile;
        }
        
        //TODO Entire Program is off by 1 index because of not incrementing currentLineSize here
        //Decide if you want to change this.
        if(charInCode == '\n'){
            break;
        }
        
        lineOfCode->codeText[lineOfCode->lineSize] = charInCode;
        ++lineOfCode->lineSize;

        checkLineOfCodeSize(lineOfCode);
    }

    lineOfCode->firstCharInLine = getFirstCharInLine(lineOfCode);

    return lineOfCode->continueReadingFile;
}

void checkLineOfCodeSize(const struct LineOfCode* lineOfCode){
    if(lineOfCodeExceeds100Chars(lineOfCode)){
        printf("Fatal error: Line %lld exceeds limit of 100 characters."
               " Correct your code and recompile.", lineOfCode->lineNumber);
        exit(1);
    }
}

bool lineOfCodeExceeds100Chars(const struct LineOfCode* lineOfCode){
    return lineOfCode->lineSize > lineOfCode->maxLineSize;
}

void auditLine(struct LineOfCode* lineOfCode){
    if(!isComment(lineOfCode)){
        matchFirstCharInLineToInstruction(lineOfCode);
    }
    else if(lineOfCode->isMultiLineComment){
        checkForEndOfMultiLineComment(lineOfCode);
    }

    if(isWhiteSpaceAtEndOfLine(lineOfCode)){
        printf("Style error: Found trailing whitespace on line %lld\n", lineOfCode->lineNumber);
    }
}

void updateLineOfCodeMetaData(struct LineOfCode* lineOfCode){
    lineOfCode->lineSize = 0;
    ++lineOfCode->lineNumber;
}

char getFirstCharInLine(const struct LineOfCode* lineOfCode){
    for(int charIndex = 0; charIndex < lineOfCode->lineSize; ++charIndex){
        if(lineOfCode->codeText[charIndex] != ' '){
            return lineOfCode->codeText[charIndex];
        }
    }

    return '\n';
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

//lineOfCode->codeText and firstCharInLine add to struct?
void matchFirstCharInLineToInstruction(struct LineOfCode* lineOfCode){
    switch(lineOfCode->firstCharInLine){
        case 'w':
        {
            if(isWhileLoop(lineOfCode)){
                auditConditionalStatementFormat(lineOfCode);
            }
        }
        break;
        case 'f':
        {
            if(isForLoop(lineOfCode)){
                auditForLoopFormat(lineOfCode);
            }
        }
        break;
        case 'i':
        {
            if(isIfStatement(lineOfCode)){
                auditConditionalStatementFormat(lineOfCode);
            }
        }
        break;
        case 'e':
        {
            if(isElseStatement(lineOfCode)){
                if(isElseIfStatement(lineOfCode)){
                    auditConditionalStatementFormat(lineOfCode);
                }
                else{
                    auditElseStatementFormat(lineOfCode);
                }
            }
        }
        break;
    }
}

void checkForEndOfMultiLineComment(struct LineOfCode* lineOfCode){
for(int charIndex = 0; charIndex < lineOfCode->lineSize-1; ++charIndex){
        if(lineOfCode->codeText[charIndex] == ' '){
            continue;
        }

        if(lineOfCode->codeText[charIndex] == '*' && lineOfCode->codeText[charIndex+1]){
            resetMultiLineCommentFlag(lineOfCode);
        }
    }
}

bool isWhiteSpaceAtEndOfLine(const struct LineOfCode* lineOfCode){
    //If increment for \n char is added this needs to change to currentLineSize-1
    if((lineOfCode->codeText[lineOfCode->lineSize-2] == ' ') && (lineOfCode->lineSize != 1)){
        return true;
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
        printf("Style error on line %lld: White space found after '('.\n", lineOfCode->lineNumber);
    }
    else if((lineOfCode->codeText[charIndex] == ')') && (lineOfCode->codeText[charIndex-1] == ' ')){
        printf("Style error on line %lld: White space found before ')'.\n", lineOfCode->lineNumber);
    }
}

void auditComparisonOperatorFormat(const struct LineOfCode* lineOfCode, const int charIndex){
     switch(lineOfCode->codeText[charIndex]){
                case '<':
                case '>':
                case '!':
                    {   
                        if(lineOfCode->codeText[charIndex-1] != ' '){
                            printf("Style error on line %lld: White space not found before comparison operator.\n", lineOfCode->lineNumber);
                        }

                        if((lineOfCode->codeText[charIndex+1] == '=' && lineOfCode->codeText[charIndex+2] != ' ')
                           || (lineOfCode->codeText[charIndex+1] != ' ')){
                            printf("Style error on line %lld: White space not found after comparison operator.\n", lineOfCode->lineNumber);
                        }
                    }
                break;
                case '=':
                    {
                        if(lineOfCode->codeText[charIndex+1] == '=' && lineOfCode->codeText[charIndex+2] != ' '){
                            printf("Style error on line %lld: White space not found after '=='.\n", lineOfCode->lineNumber);
                        }
                        else if(lineOfCode->codeText[charIndex-1] == '=' && lineOfCode->codeText[charIndex-2] != ' '){
                            printf("Style error on line %lld: White space not found before '=='.\n", lineOfCode->lineNumber);
                        }
                        else if(lineOfCode->codeText[charIndex+1] == ' ' && lineOfCode->codeText[charIndex-1] == ' '){
                            printf("Style error on line %lld: Expected comparison operator. Assignment operator found.\n", lineOfCode->lineNumber);
                        }
                    }

    }
}

bool isForLoop(const struct LineOfCode* lineOfCode){
    const char* forStringLiteral = "for(";
    
    char firstFourCharsInLineOfCode[5];
    
    int firstNonSpaceIndex = findFirstNonSpaceCharInLine(lineOfCode);
    
    grabNCharsFromString(lineOfCode, firstFourCharsInLineOfCode, sizeof(firstFourCharsInLineOfCode)-1, firstNonSpaceIndex);
    
    return isKeywordStatement(forStringLiteral, firstFourCharsInLineOfCode);
}

void auditForLoopFormat(const struct LineOfCode* lineOfCode){
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

bool isSemiColon(const char charInLineOfCode){
    if(charInLineOfCode == ';'){
        return true;
    }
    
    return false;
}

void auditSemiColonFormat(const struct LineOfCode* lineOfCode, const int charIndex){
    if(lineOfCode->codeText[charIndex-1] == ' '){
        printf("Style error on line %lld: White space found before ';' on column %d.\n", lineOfCode->lineNumber, charIndex+1);
    }
    else if(lineOfCode->codeText[charIndex+1] != ' '){
        printf("Style error on line %lld: White space not found after ';' on column %d.\n", lineOfCode->lineNumber, charIndex+1);
    }
}

bool isIfStatement(const struct LineOfCode* lineOfCode){
    const char* ifStringLiteral = "if(";

    char firstThreeCharsInLineOfCode[4];
    
    int firstNonSpaceIndex = findFirstNonSpaceCharInLine(lineOfCode);
    
    grabNCharsFromString(lineOfCode, firstThreeCharsInLineOfCode, sizeof(firstThreeCharsInLineOfCode)-1, firstNonSpaceIndex);
    
    return isKeywordStatement(ifStringLiteral, firstThreeCharsInLineOfCode);
}

bool isElseStatement(const struct LineOfCode* lineOfCode){
    const char* elseStringLiteral = "else";
    
    char firstFourCharsInLineOfCode[5];
    
    int firstNonSpaceIndex = findFirstNonSpaceCharInLine(lineOfCode);
    
    grabNCharsFromString(lineOfCode, firstFourCharsInLineOfCode, sizeof(firstFourCharsInLineOfCode)-1, firstNonSpaceIndex);
    
    return isKeywordStatement(elseStringLiteral, firstFourCharsInLineOfCode);
}

void auditElseStatementFormat(const struct LineOfCode* lineOfCode){
    for(int charInLine = 0; charInLine < lineOfCode->lineSize; ++charInLine){
        if(lineOfCode->codeText[charInLine] == '{'){
            if(lineOfCode->codeText[charInLine-1] != 'e'){
                printf("Style error on line %lld: '{' not found directly after end of else"
                    " statement.\n", lineOfCode->lineNumber);
                }
                return;
            }
        }
        
    printf("Style error on line %lld: \"{\" not found on same line as else statement.\n", lineOfCode->lineNumber);
}

bool isElseIfStatement(const struct LineOfCode* lineOfCode){
    const char* elseIfStringLiteral = "else if";
    
    char firstSevenCharsInLineOfCode[8];
    
    int firstNonSpaceIndex = findFirstNonSpaceCharInLine(lineOfCode);
    
    grabNCharsFromString(lineOfCode, firstSevenCharsInLineOfCode, sizeof(firstSevenCharsInLineOfCode)-1, firstNonSpaceIndex);
    
    return isKeywordStatement(elseIfStringLiteral, firstSevenCharsInLineOfCode);
}

bool isWhileLoop(const struct LineOfCode* lineOfCode){
    const char* whileStringLiteral = "while(";
    
    char firstSixCharsInLineOfCode[7];
    
    int firstNonSpaceIndex = findFirstNonSpaceCharInLine(lineOfCode);
    
    grabNCharsFromString(lineOfCode, firstSixCharsInLineOfCode, sizeof(firstSixCharsInLineOfCode)-1, firstNonSpaceIndex);
    
    return isKeywordStatement(whileStringLiteral, firstSixCharsInLineOfCode);
}

int findFirstNonSpaceCharInLine(const struct LineOfCode* lineOfCode){
    for(int charIndex = 0; charIndex < lineOfCode->lineSize; ++charIndex){
        if(lineOfCode->codeText[charIndex] != ' '){
            return charIndex;
        }
    }
    
    return 0;
}

//TODO rename and shrink parameters
void grabNCharsFromString(const struct LineOfCode* lineOfCode, char* charsInLineOfCode, const int charsInLineOfCodeSize, const int firstNonEmptyindex){
    for(int charIndex = 0; charIndex < charsInLineOfCodeSize; ++charIndex){
        charsInLineOfCode[charIndex] = lineOfCode->codeText[firstNonEmptyindex+charIndex];
    }
    
    charsInLineOfCode[charsInLineOfCodeSize] = '\0';
}

bool isKeywordStatement(const char* keywordLiteral, const char* beginningOfLineOfCode){
    return !(strncmp(keywordLiteral, beginningOfLineOfCode, strlen(keywordLiteral)));
}

void auditConditionalStatementFormat(const struct LineOfCode* lineOfCode){
    for(int charIndex = 0; charIndex < lineOfCode->lineSize; ++charIndex){
        if(isParenthesis(lineOfCode->codeText[charIndex])){
            auditParenthesisFormat(lineOfCode, charIndex);
        }
        else if(isComparisonOperator(lineOfCode->codeText[charIndex])){
            auditComparisonOperatorFormat(lineOfCode, charIndex);
        }
    }
}