#include <stdbool.h>
#include <stdint.h>
#include "lineInformation.h"
#include "codeFile.h"

#ifndef library_h
#define library_h

//correct order of functions
bool correctCMDLineInput(int argc);

FILE* openCodeFile(struct CodeFile* codeFile);

char getFirstCharInLine(const char* lineOfCode, const int lineSize);

bool isComment(const char* lineOfCode, const int lineSize, bool* isMultiLineComment);

bool isWhileLoop(const char* lineOfCode, const int lineSize);

bool isForLoop(const char* lineOfCode, const int lineSize);

bool checkForParenthesisAndWhiteSpace(const char* lineOfCode, const int charIndex, const long long lineNumber);

void auditConditionalStatementFormat(const char* lineOfCode, const struct LineInformation* lineInformation);

void isCorrectForLoopFormat(const char* lineOfCode, const struct LineInformation* lineInformation);

//change currentLineSize to lineSize?
bool isWhiteSpaceAtEndOfLine(const char* lineOfCode, const int8_t currentLineSize);

bool isIfStatement(const char* lineOfCode, const int lineSize);

bool isElseStatement(const char* lineOfCode, const int lineSize);

void isCorrectElseStatementFormat(const char* lineOfCode, const struct LineInformation* lineInformation);

char getFirstCharInLine(const char* lineOfCode, const int lineSize);

void matchFirstCharInLineToInstruction(const char* lineOfCode, struct LineInformation* lineInformation);

bool isElseIfStatement(const char* lineOfCode, const int lineSize);

int findFirstNonSpaceCharInLine(const char* lineOfCode, const int lineSize);

void grabCharsFromString(const char* lineOfCode, char* charsInLineOfCode, const int charsInLineOfCodeSize, const int index);

bool isKeywordStatement(const char* keywordLiteral, const char* beginningOfLineOfCode);

bool isComparisonOperator(const char charInLineOfCode);

void auditComparisonOperatorFormat(const char* lineOfCode, const int charIndex, const struct LineInformation* lineInformation);

#endif
