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

void testCharIsEndOfFileOrNewLine(){
    struct LineOfCode lineOfCode = {.codeText = "", .maxLineSize = 100, .lineSize = 0, 
    .lineNumber = 1, .continueReadingFile = true, .isMultiLineComment = false,
    .firstCharInLine = '\n'};

    TEST_ASSERT(charIsEndOfFileOrNewLine('\n', &lineOfCode) == true);
    TEST_ASSERT(charIsEndOfFileOrNewLine(' ', &lineOfCode) == false);
    TEST_ASSERT(charIsEndOfFileOrNewLine('p', &lineOfCode) == false);
}

void testIsPartOfMultiLineComment(){
    struct LineOfCode lineOfCode = {.codeText = "", .maxLineSize = 100, .lineSize = 0, 
    .lineNumber = 1, .continueReadingFile = true, .isMultiLineComment = false,
    .firstCharInLine = '\n'};

    TEST_ASSERT(isPartOfMultiLineComment(&lineOfCode) == false);

    lineOfCode.isMultiLineComment = true;

    TEST_ASSERT(isPartOfMultiLineComment(&lineOfCode) == true);
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
    lineOfCode.lineSize = 2;
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

void testIsSemiColon(){
    TEST_ASSERT(isSemiColon(';') == true);
    TEST_ASSERT(isSemiColon('\n') == false);
    TEST_ASSERT(isSemiColon(' ') == false);
    TEST_ASSERT(isSemiColon('i') == false);
}

void testIsIfStatement(){
    struct LineOfCode lineOfCode = {.codeText = "", .maxLineSize = 100, .lineSize = 0, 
    .lineNumber = 1, .continueReadingFile = true, .isMultiLineComment = false,
    .firstCharInLine = '\n'};

    TEST_ASSERT(isIfStatement(&lineOfCode) == false);

    strncpy(lineOfCode.codeText, "if(){}", sizeof(lineOfCode.codeText) - 1);
    lineOfCode.lineSize = 7;
    TEST_ASSERT(isIfStatement(&lineOfCode) == true);

    strncpy(lineOfCode.codeText, "    if(){}", sizeof(lineOfCode.codeText) - 1);
    lineOfCode.lineSize = 11;
    TEST_ASSERT(isIfStatement(&lineOfCode) == true);

    strncpy(lineOfCode.codeText, "else if(){}", sizeof(lineOfCode.codeText) - 1);
    lineOfCode.lineSize = 12;
    TEST_ASSERT(isIfStatement(&lineOfCode) == false);

    strncpy(lineOfCode.codeText, "ifFunction(){}", sizeof(lineOfCode.codeText) - 1);
    lineOfCode.lineSize = 15;
    TEST_ASSERT(isIfStatement(&lineOfCode) == false);

    strncpy(lineOfCode.codeText, "", sizeof(lineOfCode.codeText) - 1);
    lineOfCode.lineSize = 0;
    TEST_ASSERT(isIfStatement(&lineOfCode) == false);
}

void testIsElseStatement(){
    struct LineOfCode lineOfCode = {.codeText = "", .maxLineSize = 100, .lineSize = 0, 
    .lineNumber = 1, .continueReadingFile = true, .isMultiLineComment = false,
    .firstCharInLine = '\n'};

    TEST_ASSERT(isElseStatement(&lineOfCode) == false);
    
    strncpy(lineOfCode.codeText, "else {}", sizeof(lineOfCode.codeText) - 1);
    lineOfCode.lineSize = 8;
    TEST_ASSERT(isElseStatement(&lineOfCode) == true);

    strncpy(lineOfCode.codeText, "   else{}", sizeof(lineOfCode.codeText) - 1);
    lineOfCode.lineSize = 10;
    TEST_ASSERT(isElseStatement(&lineOfCode) == true);

    strncpy(lineOfCode.codeText, "else", sizeof(lineOfCode.codeText) - 1);
    lineOfCode.lineSize = 5;
    TEST_ASSERT(isElseStatement(&lineOfCode) == true);

    strncpy(lineOfCode.codeText, "//else", sizeof(lineOfCode.codeText) - 1);
    lineOfCode.lineSize = 7;
    TEST_ASSERT(isElseStatement(&lineOfCode) == false);
    
    strncpy(lineOfCode.codeText, "elseDont()", sizeof(lineOfCode.codeText) - 1);
    lineOfCode.lineSize = 11;
    TEST_ASSERT(isElseStatement(&lineOfCode) == false);
    
    strncpy(lineOfCode.codeText, "", sizeof(lineOfCode.codeText) - 1);
    lineOfCode.lineSize = 0;
    TEST_ASSERT(isElseStatement(&lineOfCode) == false);
}

void testIsElseIfStatement(){
    struct LineOfCode lineOfCode = {.codeText = "", .maxLineSize = 100, .lineSize = 0, 
    .lineNumber = 1, .continueReadingFile = true, .isMultiLineComment = false,
    .firstCharInLine = '\n'};

    TEST_ASSERT(isElseIfStatement(&lineOfCode) == false);

    strncpy(lineOfCode.codeText, "else if(){}", sizeof(lineOfCode.codeText) - 1);
    lineOfCode.lineSize = 12;
    TEST_ASSERT(isElseIfStatement(&lineOfCode) == true);
    
    strncpy(lineOfCode.codeText, "  else if(){}", sizeof(lineOfCode.codeText) - 1);
    lineOfCode.lineSize = 14;
    TEST_ASSERT(isElseIfStatement(&lineOfCode) == true);
    
    strncpy(lineOfCode.codeText, "//else if", sizeof(lineOfCode.codeText) - 1);
    lineOfCode.lineSize = 10;
    TEST_ASSERT(isElseIfStatement(&lineOfCode) == false);
    
    strncpy(lineOfCode.codeText, "/*else if", sizeof(lineOfCode.codeText) - 1);
    lineOfCode.lineSize = 10;
    TEST_ASSERT(isElseIfStatement(&lineOfCode) == false);

    strncpy(lineOfCode.codeText, "elseIfDont()", sizeof(lineOfCode.codeText) - 1);
    lineOfCode.lineSize = 11;
    TEST_ASSERT(isElseIfStatement(&lineOfCode) == false);

    strncpy(lineOfCode.codeText, "", sizeof(lineOfCode.codeText) - 1);
    lineOfCode.lineSize = 0;
    TEST_ASSERT(isElseIfStatement(&lineOfCode) == false);
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

void testFindFirstNonSpaceCharInLine(){
    struct LineOfCode lineOfCode = {.codeText = "", .maxLineSize = 100, .lineSize = 0, 
    .lineNumber = 1, .continueReadingFile = true, .isMultiLineComment = false,
    .firstCharInLine = '\n'};

    TEST_ASSERT_EQUAL_INT(findFirstNonSpaceCharInLine(&lineOfCode), 0);
    
    strncpy(lineOfCode.codeText, "   ", sizeof(lineOfCode.codeText) - 1);
    lineOfCode.lineSize = 4;
    TEST_ASSERT_EQUAL_INT(findFirstNonSpaceCharInLine(&lineOfCode), 0);

    strncpy(lineOfCode.codeText, "  f", sizeof(lineOfCode.codeText) - 1);
    lineOfCode.lineSize = 4;
    TEST_ASSERT_EQUAL_INT(findFirstNonSpaceCharInLine(&lineOfCode), 2);

    strncpy(lineOfCode.codeText, "   //", sizeof(lineOfCode.codeText) - 1);
    lineOfCode.lineSize = 6;
    TEST_ASSERT_EQUAL_INT(findFirstNonSpaceCharInLine(&lineOfCode), 3);
}

void testIsKeywordStatement(){
    TEST_ASSERT(isKeywordStatement("else", "else{}") == true);
    TEST_ASSERT(isKeywordStatement("if", "if(){}") == true);
    TEST_ASSERT(isKeywordStatement("if", "if     (){}") == true);
    TEST_ASSERT(isKeywordStatement("while", "while(){}") == true);
    TEST_ASSERT(isKeywordStatement("while", "while     (){}") == true);
    TEST_ASSERT(isKeywordStatement("else if", "else if(0){}") == true);
    TEST_ASSERT(isKeywordStatement("else if", "else{}") == false);
    TEST_ASSERT(isKeywordStatement("else", "") == false);
}

void tearDown(){

}

int main(){
    UNITY_BEGIN();

    RUN_TEST(testCorrectCMDLineInput);

    RUN_TEST(testCharIsEndOfFileOrNewLine);

    RUN_TEST(testIsPartOfMultiLineComment);

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

    RUN_TEST(testIsSemiColon);

    RUN_TEST(testIsIfStatement);

    RUN_TEST(testIsElseStatement);

    RUN_TEST(testIsElseIfStatement);

    RUN_TEST(testFindFirstNonSpaceCharInLine);

    RUN_TEST(testIsKeywordStatement);

    return UNITY_END();
}