#include <stdbool.h>
#include "library.h"
#include <string.h>

//DELETE ME when done testing.
#include <stdio.h>

bool correctCMDLineInput(int argc){
    if(argc == 2){
        return true;
    }

    //print error/help info
    //printHelp();

    return false;
}

bool isComment(char* lineOfCode, const int lineSize, bool* isMultiLineComment){
    // printf("Line is of size %d\n", lineSize);

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
            *isMultiLineComment=false;
            return true;
        }
    }

    return false;
}

bool isWhileLoop(const char* lineOfCode, const int lineSize){
    const char* whileLiteral="while";

    char placeHolderName[6];

    for(int i=0; i < lineSize; ++i){
        if(lineOfCode[i] == ' '){
                continue;
        }

        for(size_t j=0; j<strlen(whileLiteral); ++j){
            if(lineOfCode[i] == '\n'){
                puts("returned false\n");
                return false;
            }
            
            // printf("%zu ", i+j);
            placeHolderName[j]=lineOfCode[i+j];
        }

        placeHolderName[5]='\0';
        // printf("%d\n", strncmp(whileLiteral, placeHolderName, strlen(whileLiteral)));
        // printf("%s =? ", placeHolderName);
        // printf("%s\n", whileLiteral);
        return !(strncmp(whileLiteral, placeHolderName, strlen(whileLiteral)));
    }

    return false;
}