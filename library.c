#include <stdbool.h>
#include "library.h"

bool correctCMDLineInput(int argc){
    if(argc == 2){
        return true;
    }

    //print error/help info
    //printHelp();

    return false;
}

bool isComment(char* lineOfCode, int lineSize, bool* isMultiLineComment){
    for(int i=0; i < lineSize; ++i){
        if(lineOfCode[i] == ' '){
            continue;
        }
        
        //Confirm '/' can only be used for comments.
        if(lineOfCode[i] == '/'){
            if(lineOfCode[i+1] == '*'){
                *isMultiLineComment=true;
            }

            return true;
        }

        if(lineOfCode[i] == '*' && lineOfCode[i+1] == '/'){
            IsMultiLineComment=false;
            return true;
        }

        return false;
    }
}