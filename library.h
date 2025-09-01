#include <stdbool.h>
#include <stdint.h>
#include "lineInformation.h"

#ifndef library_h
#define library_h

//correct order of functions
bool correctCMDLineInput(int argc);

bool isComment(const char* lineOfCode, const int LineSize, bool* isMultiLineComment);

bool isWhileLoop(const char* lineOfCode, const int lineSize);

bool isForLoop(const char* lineOfCode, const int lineSize);

bool checkForParenthesisAndWhiteSpace(const char* lineOfCode, const int charInLine);

void isCorrectWhileLoopFormat(const char* lineOfCode, const struct LineInformation* lineInformation);

void isCorrectForLoopFormat(const char* lineOfCode, const struct LineInformation* lineInformation);

bool isWhiteSpaceAtEndOfLine(const char* lineOfCode, const int8_t currentLineSize);

bool isIfStatement(const char* lineOfCode, const int lineSize);

#endif
