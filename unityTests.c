#include "unityTestSuite/unity.h"
#include "unityTestSuite/unity.c"
#include "library.h"
#include <stdbool.h>
#include <limits.h>

void setUp(){

}

void testCorrectCMDLineInput(){
    TEST_ASSERT(correctCMDLineInput(2) == true);
    TEST_ASSERT(correctCMDLineInput(1) == false);
    TEST_ASSERT(correctCMDLineInput(INT_MAX) == false);
}

void testCheckForComments(){
    bool isMultiLineComment=true;

    TEST_ASSERT(isComment("  //", 4, &isMultiLineComment) == true);
    TEST_ASSERT(isComment("  /*", 4, &isMultiLineComment) == true);
    TEST_ASSERT(isComment("  //*", 5, &isMultiLineComment) == true);
    TEST_ASSERT(isComment("char myString[5]=\"//*\"", 16, &isMultiLineComment) == false);
    TEST_ASSERT(isComment("for()", 5, &isMultiLineComment) == false);
    TEST_ASSERT(isComment("  while()", 9, &isMultiLineComment) == false);
    TEST_ASSERT(isComment("", 0, &isMultiLineComment) == false);
}

void testIsWhileLoop(){
    TEST_ASSERT(isWhileLoop("", 0) == false);
    TEST_ASSERT(isWhileLoop("  while(1)", 10) == true);
    TEST_ASSERT(isWhileLoop("//while(1)", 10) == false);
    TEST_ASSERT(isWhileLoop("/*while(1)*/", 12) == false);
}


void tearDown(){

}

int main(){
    UNITY_BEGIN();

    RUN_TEST(testCorrectCMDLineInput);

    RUN_TEST(testCheckForComments);

    RUN_TEST(testIsWhileLoop);

    return UNITY_END();
}