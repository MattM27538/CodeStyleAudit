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