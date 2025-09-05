#include <stdio.h>

#ifndef code_file_h
#define code_file_h

struct CodeFile{
    FILE* codeFilePointer;
    const char* codeFileName;
    const char* readOnly;
};

#endif