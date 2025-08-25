#include <stdbool.h>
#include <stdint.h>

#ifndef library_h
#define library_h

bool correctCMDLineInput(int argc);

bool isComment(char* lineOfCode, int LineSize, bool* isMultiLineComment);

bool isWhileLoop(const char* lineOfCode, const int lineSize);

bool isForLoop(const char* lineOfCode, const int lineSize);

void isCorrectWhileFormat(const char* lineOfCode, int8_t currentLineSize, long long lineNumber);

#endif
