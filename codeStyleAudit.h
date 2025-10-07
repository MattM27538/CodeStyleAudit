#include <stdbool.h>
#include <stdint.h>
#include "lineOfCode.h"
#include "codeFile.h"

#ifndef library_h
#define library_h

bool correctCMDLineInput(int argc);

FILE* openCodeFile(CodeFile* codeFile);

bool readLine(FILE* codeFile, LineOfCode* lineOfCode);

char readCharsFromFile(FILE* codeFile);

bool charIsEndOfFileOrNewLine(const char charInCodeFile, LineOfCode* lineOfCode);

void addCharInCodeFileToLineOfCode(LineOfCode* lineOfCode, const char charInCodeFile);

void checkLineOfCodeSize(const LineOfCode* lineOfCode);

void setFirstCharInLine(LineOfCode* lineOfCode);

bool lineOfCodeExceeds100Chars(const LineOfCode* lineOfCode);

void auditLine(LineOfCode* lineOfCode);

bool isPartOfMultiLineComment(const LineOfCode* lineOfCode);

bool containsAssignmentOperator(const LineOfCode* lineOfCode);

void auditAssignmentOperatorFormat(const LineOfCode* lineOfCode);

void updateLineOfCodeMetaData(LineOfCode* lineOfCode);

char getFirstCharInLine(const LineOfCode* lineOfCode);

bool isComment(LineOfCode* lineOfCode);

void matchFirstCharInLineToInstruction(LineOfCode* lineOfCode);

void checkForEndOfMultiLineComment(LineOfCode* lineOfCode);

bool isWhiteSpaceAtEndOfLine(const LineOfCode* lineOfCode);

bool isStartOfMultiLineComment(const LineOfCode* lineOfCode, const int charIndex);

void setMultiLineCommentFlag(LineOfCode* lineOfCode);

bool isEndOfMultiLineComment(const LineOfCode* lineOfCode, const int charIndex);

void resetMultiLineCommentFlag(LineOfCode* lineOfCode);

bool isParenthesis(const char charInLineOfCode);

void auditParenthesisFormat(const LineOfCode* lineOfCode, const int charIndex);

void auditComparisonOperatorFormat(const LineOfCode* lineOfCode, const int charIndex);

bool isForLoop(const LineOfCode* lineOfCode);

void auditForLoopFormat(const LineOfCode* lineOfCode);

bool isComparisonOperator(const char charInLineOfCode);

bool isSemiColon(const char charInLineOfCode);

void auditSemiColonFormat(const LineOfCode* lineOfCode, const int charIndex);

bool isIfStatement(const LineOfCode* lineOfCode);

bool isElseStatement(const LineOfCode* lineOfCode);

void auditElseStatementFormat(const LineOfCode* lineOfCode);

bool isElseIfStatement(const LineOfCode* lineOfCode);

bool isWhileLoop(const LineOfCode* lineOfCode);

int findFirstNonSpaceCharInLine(const LineOfCode* lineOfCode);

void grabNCharsFromString(const LineOfCode* lineOfCode, char* charsInLineOfCode, const int charsInLineOfCodeSize, const int index);

bool isKeywordStatement(const char* keywordLiteral, const char* beginningOfLineOfCode);

void auditConditionalStatementFormat(const LineOfCode* lineOfCode);

#endif