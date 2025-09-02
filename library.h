#include <stdbool.h>
#include <stdint.h>
#include "lineInformation.h"

#ifndef library_h
#define library_h

//correct order of functions
char getFirstCharInLine(const char* lineOfCode, const int lineSize);

bool correctCMDLineInput(int argc);

bool isComment(const char* lineOfCode, const int lineSize, bool* isMultiLineComment);

bool isWhileLoop(const char* lineOfCode, const int lineSize);

bool isForLoop(const char* lineOfCode, const int lineSize);

bool checkForParenthesisAndWhiteSpace(const char* lineOfCode, const int charInLine);

void isCorrectWhileLoopFormat(const char* lineOfCode, const struct LineInformation* lineInformation);

void isCorrectForLoopFormat(const char* lineOfCode, const struct LineInformation* lineInformation);

//change currentLineSize to lineSize?
bool isWhiteSpaceAtEndOfLine(const char* lineOfCode, const int8_t currentLineSize);

bool isIfStatement(const char* lineOfCode, const int lineSize);

bool isElseStatement(const char* lineOfCode, const int lineSize);

char getFirstCharInLine(const char* lineOfCode, const int lineSize);

#endif
