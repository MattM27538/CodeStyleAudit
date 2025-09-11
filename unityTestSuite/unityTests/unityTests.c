#include "../unity.h"
#include "../unity.c"
#include "../../library.h"
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

void testIsWhiteSpaceAtEndOfLine(){
    TEST_ASSERT(isWhiteSpaceAtEndOfLine("", 1) == false);
    TEST_ASSERT(isWhiteSpaceAtEndOfLine("  ", 3) == true);
    TEST_ASSERT(isWhiteSpaceAtEndOfLine("while(){}", 10) == false);
    TEST_ASSERT(isWhiteSpaceAtEndOfLine("while(){} ", 11) == true);
}

void testIsElseIfStatement(){
    TEST_ASSERT(isElseIfStatement("else if(){}", 11) == true);
    TEST_ASSERT(isElseIfStatement("else{}", 6) == false);
    TEST_ASSERT(isElseIfStatement("elseDont()", 10) == false);
    TEST_ASSERT(isElseIfStatement("", 0) == false);
    TEST_ASSERT(isElseIfStatement("  ", 2) == false);
}

void testIsIfStatement(){
    TEST_ASSERT(isIfStatement("if(){}", 6) == true);
    TEST_ASSERT(isIfStatement("    if(){}", 10) == true);
    TEST_ASSERT(isIfStatement("else if(){}", 11) == false);
    TEST_ASSERT(isIfStatement("ifFunction(){}", 18) == false );
    TEST_ASSERT(isIfStatement("", 0) == false);
}

void tearDown(){

}

int main(){
    UNITY_BEGIN();

    RUN_TEST(testCorrectCMDLineInput);

    RUN_TEST(testCheckForComments);

    RUN_TEST(testIsWhileLoop);
    
    RUN_TEST(testIsWhiteSpaceAtEndOfLine);

    RUN_TEST(testIsIfStatement);

    RUN_TEST(testIsElseIfStatement);

    return UNITY_END();
}