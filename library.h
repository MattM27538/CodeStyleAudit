#include <stdbool.h>

#ifndef library_h
#define library_h

bool correctCMDLineInput(int argc);

bool isComment(char* lineOfCode, int LineSize, bool* isMultiLineComment);

#endif
