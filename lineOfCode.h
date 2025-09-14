#include <stdint.h>

#ifndef lineOfCode_h
#define lineOfCode_h

//Consider making this a singleton
//TODO split into two classes? Or Remove unlinked variables.
//TODO change name to lineoFCode add lineOfCode to struct as text or some other name
struct LineOfCode{
    char codeText[100];
    const int8_t maxLineSize;
    int8_t currentLineSize;
    long long lineNumber;
    bool continueReadingFile;
    bool isMultiLineComment;
    char firstCharInLine;
};

#endif