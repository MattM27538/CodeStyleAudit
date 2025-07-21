#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "library.h"


int main(int argc, char* argv[]){
    if(!correctCMDLineInput(argc)){
        exit(1);
    }

    return 0;
}
