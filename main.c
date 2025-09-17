#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "LineOfCode.h"
#include "library.h"
#include <assert.h>
#include <stdint.h>

int main(int argc, char* argv[]){
    if(!correctCMDLineInput(argc)){
        return EXIT_FAILURE;
    }

    struct CodeFile codeFile = {.codeFilePointer = NULL,
           .codeFileName = argv[1], .readOnly = "r"};

    struct LineOfCode lineOfCode = {.codeText = "", .maxLineSize = 100, .lineSize = 0, 
           .lineNumber = 1, .continueReadingFile = true, .isMultiLineComment = false,
           .firstCharInLine = '\n'};

    codeFile.codeFilePointer = openCodeFile(&codeFile);

    while(readLine(codeFile.codeFilePointer, &lineOfCode));
    
    fclose(codeFile.codeFilePointer);

    return 0;
}
