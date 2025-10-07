#include <stdbool.h>
#include "codeStyleAudit.h"
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

bool correctCMDLineInput(int argc){
    if(argc == 2){
        return true;
    }

    fprintf(stderr, "Incorrect number of command line arguments found. Expected 2, found"
            " %d.\nExiting.", argc);

    return false;
}

FILE* openCodeFile(CodeFile* codeFile){
    FILE* codeFilePointer = fopen(codeFile->codeFileName, codeFile->readOnly);
    
    if(codeFilePointer == NULL){
        perror("Error opening code file. Please try again.");
        exit(1);
    }

    return codeFilePointer;
}

bool readLine(FILE* codeFile, LineOfCode* lineOfCode){
    char charInCodeFile;

    for(int i = 0; i < lineOfCode->maxLineSize + 1; ++i){
        charInCodeFile = readCharsFromFile(codeFile);
        
        if(charIsEndOfFileOrNewLine(charInCodeFile, lineOfCode)){
            break;
        }
        
        addCharInCodeFileToLineOfCode(lineOfCode, charInCodeFile);

        checkLineOfCodeSize(lineOfCode);
    }

    setFirstCharInLine(lineOfCode);

    return lineOfCode->continueReadingFile;
}

char readCharsFromFile(FILE* codeFile){
    char charInCodeFile = fgetc(codeFile);

    return charInCodeFile;
}

bool charIsEndOfFileOrNewLine(const char charInCodeFile, LineOfCode* lineOfCode){
    if(charInCodeFile == EOF){
        lineOfCode->continueReadingFile = false;
    }
    return (charInCodeFile == EOF || charInCodeFile =='\n');
}

void addCharInCodeFileToLineOfCode(LineOfCode* lineOfCode, const char charInCodeFile){
    lineOfCode->codeText[lineOfCode->lineSize] = charInCodeFile;
    ++lineOfCode->lineSize;
}

void checkLineOfCodeSize(const LineOfCode* lineOfCode){
    if(lineOfCodeExceeds100Chars(lineOfCode)){
        printf("Fatal error: Line %lld exceeds limit of 100 characters. "
               "Correct your code and recompile.", lineOfCode->lineNumber);
        exit(1);
    }
}

void setFirstCharInLine(LineOfCode* lineOfCode){
    lineOfCode->firstCharInLine = getFirstCharInLine(lineOfCode);
}

bool lineOfCodeExceeds100Chars(const LineOfCode* lineOfCode){
    return lineOfCode->lineSize > lineOfCode->maxLineSize;
}

void auditLine(LineOfCode* lineOfCode){
    if(!isComment(lineOfCode)){
        matchFirstCharInLineToInstruction(lineOfCode);
        if(containsAssignmentOperator(lineOfCode)){
            auditAssignmentOperatorFormat(lineOfCode);
        }
    }
    else if(isPartOfMultiLineComment(lineOfCode)){
        checkForEndOfMultiLineComment(lineOfCode);
    }

    if(isWhiteSpaceAtEndOfLine(lineOfCode)){
        printf("Style error: Found trailing whitespace on line %lld\n", lineOfCode->lineNumber);
    }
}

bool isPartOfMultiLineComment(const LineOfCode* lineOfCode){
    return lineOfCode->isMultiLineComment;
}

bool containsAssignmentOperator(const LineOfCode* lineOfCode){
    for(int charIndex = 0; charIndex < lineOfCode->lineSize; ++charIndex){
        if(lineOfCode->codeText[charIndex] == '='){
            if(isComparisonOperator(lineOfCode->codeText[charIndex - 1]) 
               || (lineOfCode->codeText[charIndex + 1] == '=')){
                return false;
            }

            return true;
        }
    }

    return false;
}

void auditAssignmentOperatorFormat(const LineOfCode* lineOfCode){
    for(int charIndex = 0; charIndex < lineOfCode->lineSize; ++charIndex){
        if(lineOfCode->codeText[charIndex] == '='){
            if(lineOfCode->codeText[charIndex - 1] != ' '){
                printf("Style error on line %lld: White space not found before "
                       "assignment operator.\n", lineOfCode->lineNumber);
            }
            if(lineOfCode->codeText[charIndex + 1] != ' '){
                printf("Style error on line %lld: White space not found after "
                       "assignment operator.\n", lineOfCode->lineNumber);
            }
        }
    }
}

void updateLineOfCodeMetaData(LineOfCode* lineOfCode){
    lineOfCode->lineSize = 0;
    ++lineOfCode->lineNumber;
}

char getFirstCharInLine(const LineOfCode* lineOfCode){
    for(int charIndex = 0; charIndex < lineOfCode->lineSize; ++charIndex){
        if(lineOfCode->codeText[charIndex] != ' '){
            return lineOfCode->codeText[charIndex];
        }
    }

    return '\n';
}

bool isComment(LineOfCode* lineOfCode){
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

void matchFirstCharInLineToInstruction(LineOfCode* lineOfCode){
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

void checkForEndOfMultiLineComment(LineOfCode* lineOfCode){
for(int charIndex = 0; charIndex < lineOfCode->lineSize-1; ++charIndex){
        if(lineOfCode->codeText[charIndex] == ' '){
            continue;
        }

        if(lineOfCode->codeText[charIndex] == '*' && lineOfCode->codeText[charIndex+1]){
            resetMultiLineCommentFlag(lineOfCode);
        }
    }
}

bool isWhiteSpaceAtEndOfLine(const LineOfCode* lineOfCode){
    if((lineOfCode->codeText[lineOfCode->lineSize-2] == ' ') && (lineOfCode->lineSize != 1)){
        return true;
    }

    return false;
}

bool isStartOfMultiLineComment(const LineOfCode* lineOfCode, const int charIndex){
    if(lineOfCode->codeText[charIndex + 1] == '*' ){
        return true;
    }

    return false;
}

void setMultiLineCommentFlag(LineOfCode* lineOfCode){
    lineOfCode->isMultiLineComment = true;
}

bool isEndOfMultiLineComment(const LineOfCode* lineOfCode, const int charIndex){
    if(lineOfCode->codeText[charIndex - 1] == '*'){
        return true;
    }

    return false;
}

void resetMultiLineCommentFlag(LineOfCode* lineOfCode){
    lineOfCode->isMultiLineComment = false;
}

bool isParenthesis(const char charInLineOfCode){
    if((charInLineOfCode == ')') || (charInLineOfCode == '(')){
        return true;
    }

    return false;
}

void auditParenthesisFormat(const LineOfCode* lineOfCode, const int charIndex){
    if(((lineOfCode->codeText[charIndex] == '(') && (lineOfCode->codeText[charIndex+1] == ' '))){
        printf("Style error on line %lld: White space found after '('.\n", lineOfCode->lineNumber);
    }
    else if((lineOfCode->codeText[charIndex] == ')') 
             && (lineOfCode->codeText[charIndex-1] == ' ')){
                printf("Style error on line %lld: White space "
                    "found before ')'.\n", lineOfCode->lineNumber);
    }
}

void auditComparisonOperatorFormat(const LineOfCode* lineOfCode, const int charIndex){
     switch(lineOfCode->codeText[charIndex]){
        case '<':
        case '>':
        case '!':
            {   
                if(lineOfCode->codeText[charIndex-1] != ' '){
                    printf("Style error on line %lld: White space not found before "
                            "comparison operator.\n", lineOfCode->lineNumber);
                }

                if((lineOfCode->codeText[charIndex+1] == '=' 
                    && lineOfCode->codeText[charIndex+2] != ' ')
                    || (lineOfCode->codeText[charIndex+1] != ' ')){
                        printf("Style error on line %lld: White space not found after " 
                                "comparison operator.\n", lineOfCode->lineNumber);
                }
            }
        break;
        case '=':
            {
                if(lineOfCode->codeText[charIndex+1] == '=' 
                    && lineOfCode->codeText[charIndex+2] != ' '){
                        printf("Style error on line %lld: White space not "
                                "found after '=='.\n", lineOfCode->lineNumber);
                }
                else if(lineOfCode->codeText[charIndex-1] == '=' 
                    && lineOfCode->codeText[charIndex-2] != ' '){
                        printf("Style error on line %lld: White space "
                                "not found before '=='.\n", lineOfCode->lineNumber);
                }
                else if(lineOfCode->codeText[charIndex+1] == ' ' 
                    && lineOfCode->codeText[charIndex-1] == ' '){
                        printf("Style error on line %lld: Expected comparison operator. "
                                "Assignment operator found.\n", lineOfCode->lineNumber);
                }
            }

    }
}

bool isForLoop(const LineOfCode* lineOfCode){
    const char* forStringLiteral = "for(";
    
    char firstFourCharsInLineOfCode[5];
    
    int firstNonSpaceIndex = findFirstNonSpaceCharInLine(lineOfCode);
    
    grabNCharsFromString(lineOfCode, firstFourCharsInLineOfCode, 
    sizeof(firstFourCharsInLineOfCode)-1, firstNonSpaceIndex);
    
    return isKeywordStatement(forStringLiteral, firstFourCharsInLineOfCode);
}

void auditForLoopFormat(const LineOfCode* lineOfCode){
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

void auditSemiColonFormat(const LineOfCode* lineOfCode, const int charIndex){
    if(lineOfCode->codeText[charIndex-1] == ' '){
        printf("Style error on line %lld: White space found before ';' "
               "on column %d.\n", lineOfCode->lineNumber, charIndex+1);
    }
    else if(lineOfCode->codeText[charIndex+1] != ' '){
        printf("Style error on line %lld: White space not found after ';' "
               "on column %d.\n", lineOfCode->lineNumber, charIndex+1);
    }
}

bool isIfStatement(const LineOfCode* lineOfCode){
    const char* ifStringLiteral = "if(";

    char firstThreeCharsInLineOfCode[4];
    
    int firstNonSpaceIndex = findFirstNonSpaceCharInLine(lineOfCode);
    
    grabNCharsFromString(lineOfCode, firstThreeCharsInLineOfCode, 
    sizeof(firstThreeCharsInLineOfCode)-1, firstNonSpaceIndex);
    
    return isKeywordStatement(ifStringLiteral, firstThreeCharsInLineOfCode);
}

bool isElseStatement(const LineOfCode* lineOfCode){
    const char* elseStringLiteral = "else";
    
    char firstFourCharsInLineOfCode[5];
    
    int firstNonSpaceIndex = findFirstNonSpaceCharInLine(lineOfCode);
    
    grabNCharsFromString(lineOfCode, firstFourCharsInLineOfCode, 
    sizeof(firstFourCharsInLineOfCode)-1, firstNonSpaceIndex);
    
    return isKeywordStatement(elseStringLiteral, firstFourCharsInLineOfCode);
}

void auditElseStatementFormat(const LineOfCode* lineOfCode){
    for(int charInLine = 0; charInLine < lineOfCode->lineSize; ++charInLine){
        if(lineOfCode->codeText[charInLine] == '{'){
            if(lineOfCode->codeText[charInLine-1] != 'e'){
                printf("Style error on line %lld: '{' not found directly after end of else"
                    " statement.\n", lineOfCode->lineNumber);
                }
                return;
            }
        }
        
    printf("Style error on line %lld: \"{\" not found on same line "
           "as else statement.\n", lineOfCode->lineNumber);
}

bool isElseIfStatement(const LineOfCode* lineOfCode){
    const char* elseIfStringLiteral = "else if";
    
    char firstSevenCharsInLineOfCode[8];
    
    int firstNonSpaceIndex = findFirstNonSpaceCharInLine(lineOfCode);
    
    grabNCharsFromString(lineOfCode, firstSevenCharsInLineOfCode, 
    sizeof(firstSevenCharsInLineOfCode)-1, firstNonSpaceIndex);
    
    return isKeywordStatement(elseIfStringLiteral, firstSevenCharsInLineOfCode);
}

bool isWhileLoop(const LineOfCode* lineOfCode){
    const char* whileStringLiteral = "while(";
    
    char firstSixCharsInLineOfCode[7];
    
    int firstNonSpaceIndex = findFirstNonSpaceCharInLine(lineOfCode);
    
    grabNCharsFromString(lineOfCode, firstSixCharsInLineOfCode, 
    sizeof(firstSixCharsInLineOfCode)-1, firstNonSpaceIndex);
    
    return isKeywordStatement(whileStringLiteral, firstSixCharsInLineOfCode);
}

int findFirstNonSpaceCharInLine(const LineOfCode* lineOfCode){
    for(int charIndex = 0; charIndex < lineOfCode->lineSize; ++charIndex){
        if(lineOfCode->codeText[charIndex] != ' '){
            return charIndex;
        }
    }
    
    return 0;
}

void grabNCharsFromString(const LineOfCode* lineOfCode, char* charsInLineOfCode, 
const int charsInLineOfCodeSize, const int firstNonEmptyindex){
    for(int charIndex = 0; charIndex < charsInLineOfCodeSize; ++charIndex){
        charsInLineOfCode[charIndex] = lineOfCode->codeText[firstNonEmptyindex+charIndex];
    }
    
    charsInLineOfCode[charsInLineOfCodeSize] = '\0';
}

bool isKeywordStatement(const char* keywordLiteral, const char* beginningOfLineOfCode){
    return !(strncmp(keywordLiteral, beginningOfLineOfCode, strlen(keywordLiteral)));
}

void auditConditionalStatementFormat(const LineOfCode* lineOfCode){
    for(int charIndex = 0; charIndex < lineOfCode->lineSize; ++charIndex){
        if(isParenthesis(lineOfCode->codeText[charIndex])){
            auditParenthesisFormat(lineOfCode, charIndex);
        }
        else if(isComparisonOperator(lineOfCode->codeText[charIndex])){
            auditComparisonOperatorFormat(lineOfCode, charIndex);
        }
    }
}