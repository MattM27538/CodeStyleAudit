#include "../unity.h"
#include "../unity.c"
#include "../../codeStyleAudit.h"
#include <stdbool.h>
#include <limits.h>
#include <string.h>
#include "../../lineOfCode.h"

void setUp(){}

void testCorrectCMDLineInput(){
    TEST_ASSERT(correctCMDLineInput(2) == true);
    TEST_ASSERT(correctCMDLineInput(1) == false);
    TEST_ASSERT(correctCMDLineInput(0) == false);
    TEST_ASSERT(correctCMDLineInput(INT_MAX) == false);
}

void testGetFirstCharInLine(){
    struct LineOfCode lineOfCode = {.codeText = "", .maxLineSize = 100, .lineSize = 0, 
    .lineNumber = 1, .continueReadingFile = true, .isMultiLineComment = false,
    .firstCharInLine = '\n'};

    TEST_ASSERT_EQUAL_CHAR(getFirstCharInLine(&lineOfCode), '\n');

    strncpy(lineOfCode.codeText, "  for", sizeof(lineOfCode.codeText) - 1);
    lineOfCode.lineSize = 6;

    TEST_ASSERT_EQUAL_CHAR(getFirstCharInLine(&lineOfCode), 'f');

    strncpy(lineOfCode.codeText, "  ", sizeof(lineOfCode.codeText) - 1);
    lineOfCode.lineSize = 3;
    TEST_ASSERT_EQUAL_CHAR(getFirstCharInLine(&lineOfCode), '\n');

    strncpy(lineOfCode.codeText, "//for", sizeof(lineOfCode.codeText) - 1);
    lineOfCode.lineSize = 6;
    TEST_ASSERT_EQUAL_CHAR(getFirstCharInLine(&lineOfCode), '/');
}

void testIsComment(){
    struct LineOfCode lineOfCode = {.codeText = "", .maxLineSize = 100, .lineSize = 0, 
    .lineNumber = 1, .continueReadingFile = true, .isMultiLineComment = false,
    .firstCharInLine = '\n'};

    TEST_ASSERT(isComment(&lineOfCode) == false);

    strncpy(lineOfCode.codeText, "  ", sizeof(lineOfCode.codeText) - 1);
    lineOfCode.lineSize = 3;
    TEST_ASSERT(isComment(&lineOfCode) == false);
    
    strncpy(lineOfCode.codeText, "//", sizeof(lineOfCode.codeText) - 1);
    lineOfCode.lineSize = 3;
    TEST_ASSERT(isComment(&lineOfCode) == true);
    
    strncpy(lineOfCode.codeText, "/*", sizeof(lineOfCode.codeText) - 1);
    lineOfCode.lineSize = 3;
    TEST_ASSERT(isComment(&lineOfCode) == true);
    
    strncpy(lineOfCode.codeText, "  //", sizeof(lineOfCode.codeText) - 1);
    lineOfCode.lineSize = 5;
    TEST_ASSERT(isComment(&lineOfCode) == true);
    
    strncpy(lineOfCode.codeText, "  */", sizeof(lineOfCode.codeText) - 1);
    lineOfCode.lineSize = 5;
    TEST_ASSERT(isComment(&lineOfCode) == true);
    
    strncpy(lineOfCode.codeText, "for(){}//", sizeof(lineOfCode.codeText) - 1);
    lineOfCode.lineSize = 10;
    TEST_ASSERT(isComment(&lineOfCode) == true);
    
    strncpy(lineOfCode.codeText, "for(){}*/", sizeof(lineOfCode.codeText) - 1);
    lineOfCode.lineSize = 10;
    TEST_ASSERT(isComment(&lineOfCode) == true);
}

void testIsWhiteSpaceAtEndOfLine(){
    struct LineOfCode lineOfCode = {.codeText = "", .maxLineSize = 100, .lineSize = 0, 
    .lineNumber = 1, .continueReadingFile = true, .isMultiLineComment = false,
    .firstCharInLine = '\n'};

    TEST_ASSERT(isWhiteSpaceAtEndOfLine(&lineOfCode) == false);
    
    strncpy(lineOfCode.codeText, "  ", sizeof(lineOfCode.codeText) - 1);
    lineOfCode.lineSize = 3;
    TEST_ASSERT(isWhiteSpaceAtEndOfLine(&lineOfCode) == true);

    strncpy(lineOfCode.codeText, "while(){}", sizeof(lineOfCode.codeText) - 1);
    lineOfCode.lineSize = 10;
    TEST_ASSERT(isWhiteSpaceAtEndOfLine(&lineOfCode) == false);

    strncpy(lineOfCode.codeText, "while(){} ", sizeof(lineOfCode.codeText) - 1);
    lineOfCode.lineSize = 11;
    TEST_ASSERT(isWhiteSpaceAtEndOfLine(&lineOfCode) == true);
}

void testIsStartOfMultiLineComment(){
    struct LineOfCode lineOfCode = {.codeText = "", .maxLineSize = 100, .lineSize = 0, 
    .lineNumber = 1, .continueReadingFile = true, .isMultiLineComment = false,
    .firstCharInLine = '\n'};

    TEST_ASSERT(isStartOfMultiLineComment(&lineOfCode, 0) == false);

    strncpy(lineOfCode.codeText, "/* ", sizeof(lineOfCode.codeText) - 1);
    lineOfCode.lineSize = 4;
    TEST_ASSERT(isStartOfMultiLineComment(&lineOfCode, 0) == true);

    strncpy(lineOfCode.codeText, "// ", sizeof(lineOfCode.codeText) - 1);
    lineOfCode.lineSize = 4;
    TEST_ASSERT(isStartOfMultiLineComment(&lineOfCode, 0) == false);

    strncpy(lineOfCode.codeText, "    /* ", sizeof(lineOfCode.codeText) - 1);
    lineOfCode.lineSize = 8;
    TEST_ASSERT(isStartOfMultiLineComment(&lineOfCode, 4) == true);

    strncpy(lineOfCode.codeText, "   //* ", sizeof(lineOfCode.codeText) - 1);
    lineOfCode.lineSize = 8;
    TEST_ASSERT(isStartOfMultiLineComment(&lineOfCode, 3) == false);
}

void testIsEndOfMultiLineComment(){
    struct LineOfCode lineOfCode = {.codeText = "", .maxLineSize = 100, .lineSize = 0, 
    .lineNumber = 1, .continueReadingFile = true, .isMultiLineComment = false,
    .firstCharInLine = '\n'};

    TEST_ASSERT(isEndOfMultiLineComment(&lineOfCode, 0) == false);

    strncpy(lineOfCode.codeText, "*/ ", sizeof(lineOfCode.codeText) - 1);
    lineOfCode.lineSize = 4;
    TEST_ASSERT(isEndOfMultiLineComment(&lineOfCode, 1) == true);

    strncpy(lineOfCode.codeText, "  // ", sizeof(lineOfCode.codeText) - 1);
    lineOfCode.lineSize = 6;
    TEST_ASSERT(isEndOfMultiLineComment(&lineOfCode, 2) == false);

    strncpy(lineOfCode.codeText, "    */ ", sizeof(lineOfCode.codeText) - 1);
    lineOfCode.lineSize = 8;
    TEST_ASSERT(isEndOfMultiLineComment(&lineOfCode, 5) == true);

    strncpy(lineOfCode.codeText, "   //*/ ", sizeof(lineOfCode.codeText) - 1);
    lineOfCode.lineSize = 9;
    TEST_ASSERT(isEndOfMultiLineComment(&lineOfCode, 3) == false);
}

void testIsParenthesis(){
    TEST_ASSERT(isParenthesis('(') == true);
    TEST_ASSERT(isParenthesis(')') == true);
    TEST_ASSERT(isParenthesis(' ') == false);
    TEST_ASSERT(isParenthesis('\n') == false);
}

void testIsForLoop(){
    struct LineOfCode lineOfCode = {.codeText = "", .maxLineSize = 100, .lineSize = 0, 
    .lineNumber = 1, .continueReadingFile = true, .isMultiLineComment = false,
    .firstCharInLine = '\n'};

    TEST_ASSERT(isForLoop(&lineOfCode) == false);

    strncpy(lineOfCode.codeText, "for(;;){}", sizeof(lineOfCode.codeText) - 1);
    lineOfCode.lineSize = 10;
    TEST_ASSERT(isForLoop(&lineOfCode) == true);

    strncpy(lineOfCode.codeText, "forEach(){}", sizeof(lineOfCode.codeText) - 1);
    lineOfCode.lineSize = 12;
    TEST_ASSERT(isForLoop(&lineOfCode) == false);

    strncpy(lineOfCode.codeText, "  for(;;){}", sizeof(lineOfCode.codeText) - 1);
    lineOfCode.lineSize = 12;
    TEST_ASSERT(isForLoop(&lineOfCode) == true);

    strncpy(lineOfCode.codeText, "   ", sizeof(lineOfCode.codeText) - 1);
    lineOfCode.lineSize = 11;
    TEST_ASSERT(isForLoop(&lineOfCode) == false);

    strncpy(lineOfCode.codeText, "//for", sizeof(lineOfCode.codeText) - 1);
    lineOfCode.lineSize = 16;
    TEST_ASSERT(isForLoop(&lineOfCode) == false);
}

void testIsComparisonOperator(){
    TEST_ASSERT(isComparisonOperator('<') == true);
    TEST_ASSERT(isComparisonOperator('>') == true);
    TEST_ASSERT(isComparisonOperator('=') == true);
    TEST_ASSERT(isComparisonOperator('!') == true);
    TEST_ASSERT(isComparisonOperator(' ') == false);
    TEST_ASSERT(isComparisonOperator('p') == false);
}

void testIsWhileLoop(){
    struct LineOfCode lineOfCode = {.codeText = "", .maxLineSize = 100, .lineSize = 0, 
    .lineNumber = 1, .continueReadingFile = true, .isMultiLineComment = false,
    .firstCharInLine = '\n'};

    TEST_ASSERT(isWhileLoop(&lineOfCode) == false);

    strncpy(lineOfCode.codeText, "  while(1)", sizeof(lineOfCode.codeText) - 1);
    lineOfCode.lineSize = 11;
    TEST_ASSERT(isWhileLoop(&lineOfCode) == true);

    strncpy(lineOfCode.codeText, "//while(1)", sizeof(lineOfCode.codeText) - 1);
    TEST_ASSERT(isWhileLoop(&lineOfCode) == false);
    
    strncpy(lineOfCode.codeText, "/*while(1)*/", sizeof(lineOfCode.codeText) - 1);
    lineOfCode.lineSize = 13;
    TEST_ASSERT(isWhileLoop(&lineOfCode) == false);
}

// void testIsWhiteSpaceAtEndOfLine(){
    // strncpy(lineOfCode.codeText, , sizeof(lineOfCode.codeText) - 1);
//     TEST_ASSERT(isWhiteSpaceAtEndOfLine("", 1) == false);
//     TEST_ASSERT(isWhiteSpaceAtEndOfLine("  ", 3) == true);
//     TEST_ASSERT(isWhiteSpaceAtEndOfLine("while(){}", 10) == false);
//     TEST_ASSERT(isWhiteSpaceAtEndOfLine("while(){} ", 11) == true);
// }

// void testIsElseIfStatement(){
//     TEST_ASSERT(isElseIfStatement("else if(){}", 11) == true);
//     TEST_ASSERT(isElseIfStatement("else{}", 6) == false);
//     TEST_ASSERT(isElseIfStatement("elseDont()", 10) == false);
//     TEST_ASSERT(isElseIfStatement("", 0) == false);
//     TEST_ASSERT(isElseIfStatement("  ", 2) == false);
// }

// void testIsIfStatement(){
//     TEST_ASSERT(isIfStatement("if(){}", 6) == true);
//     TEST_ASSERT(isIfStatement("    if(){}", 10) == true);
//     TEST_ASSERT(isIfStatement("else if(){}", 11) == false);
//     TEST_ASSERT(isIfStatement("ifFunction(){}", 18) == false );
//     TEST_ASSERT(isIfStatement("", 0) == false);
// }

void tearDown(){

}

int main(){
    UNITY_BEGIN();

    RUN_TEST(testCorrectCMDLineInput);

    RUN_TEST(testGetFirstCharInLine);

    RUN_TEST(testIsComment);

    RUN_TEST(testIsWhiteSpaceAtEndOfLine);

    RUN_TEST(testIsStartOfMultiLineComment);

    RUN_TEST(testIsWhileLoop);

    RUN_TEST(testIsStartOfMultiLineComment);

    RUN_TEST(testIsEndOfMultiLineComment);
    
    RUN_TEST(testIsParenthesis);

    RUN_TEST(testIsForLoop);

    RUN_TEST(testIsComparisonOperator);

    // RUN_TEST(testIsIfStatement);

    // RUN_TEST(testIsElseIfStatement);

    // RUN__TEST(testIsForLoop);

    return UNITY_END();
}