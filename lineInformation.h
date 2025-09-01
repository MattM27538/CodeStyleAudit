#include <stdint.h>

#ifndef lineInformation_h
#define lineInformation_h

//Consider making this a singleton
struct LineInformation{
    const int8_t maxLineSize;
    int8_t currentLineSize;
    long long lineNumber;
    bool continueReadingFile;
    bool isMultiLineComment;
};

#endif