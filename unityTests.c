#include "unityTestSuite/unity.h"
#include "unityTestSuite/unity.c"
#include "library.h"
#include <stdbool.h>
#include <limits.h>

void setUp(){

}

void test_correctCMDLineInput(){
    TEST_ASSERT(correctCMDLineInput(2) == true);
    TEST_ASSERT(correctCMDLineInput(1) == false);
    TEST_ASSERT(correctCMDLineInput(INT_MAX) == false);
}

void test_checkForComments(){
    bool trueBool=true;

    TEST_ASSERT(isComment("  //", 4, &trueBool) == true);
    TEST_ASSERT(isComment("  /*", 4, &trueBool) == true);
    TEST_ASSERT(isComment("  //*", 5, &trueBool) == true);
    TEST_ASSERT(isComment("char myString[5]=\"//*\"", 16, &trueBool) == false);
    TEST_ASSERT(isComment("for()", 5, &trueBool) == false);
    TEST_ASSERT(isComment("  while()", 9, &trueBool) == false);
    TEST_ASSERT(isComment("", 0, &trueBool) == false);
}


void tearDown(){

}

int main(){
    UNITY_BEGIN();

    RUN_TEST(test_correctCMDLineInput);

    RUN_TEST(test_checkForComments);

    return UNITY_END();
}