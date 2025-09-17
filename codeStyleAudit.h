#include <stdbool.h>
#include <stdint.h>
#include "lineOfCode.h"
#include "codeFile.h"

#ifndef library_h
#define library_h

//correct order of functions
bool correctCMDLineInput(int argc);

FILE* openCodeFile(struct CodeFile* codeFile);

bool readLine(FILE* codeFile, struct LineOfCode* lineOfCode);

char getFirstCharInLine(const struct LineOfCode* lineOfCode);

void matchFirstCharInLineToInstruction(struct LineOfCode* lineOfCode);

bool isWhiteSpaceAtEndOfLine(const struct LineOfCode* lineOfCode);

bool isComment(struct LineOfCode* lineOfCode);

bool isStartOfMultiLineComment(const struct LineOfCode* lineOfCode, const int charIndex);

void setMultiLineCommentFlag(struct LineOfCode* lineOfCode);

bool isEndOfMultiLineComment(const struct LineOfCode* lineOfCode, const int charIndex);

void resetMultiLineCommentFlag(struct LineOfCode* lineOfCode);

bool isParenthesis(const char charInLineOfCode);

void auditParenthesisFormat(const struct LineOfCode* lineOfCode, const int charIndex);

bool isComparisonOperator(const char charInLineOfCode);

void auditComparisonOperatorFormat(const struct LineOfCode* lineOfCode, const int charIndex);

bool isForLoop(const struct LineOfCode* lineOfCode);

void auditForLoopFormat(const struct LineOfCode* lineOfCode);

bool isSemiColon(const char charInLineOfCode);

void auditSemiColonFormat(const struct LineOfCode* lineOfCode, const int charIndex);

bool isIfStatement(const struct LineOfCode* lineOfCode);

bool isElseStatement(const struct LineOfCode* lineOfCode);

void auditElseStatementFormat(const struct LineOfCode* lineOfCode);

bool isElseIfStatement(const struct LineOfCode* lineOfCode);

bool isWhileLoop(const struct LineOfCode* lineOfCode);

int findFirstNonSpaceCharInLine(const struct LineOfCode* lineOfCode);

void grabCharsFromString(const struct LineOfCode* lineOfCode, char* charsInLineOfCode, const int charsInLineOfCodeSize, const int index);

bool isKeywordStatement(const char* keywordLiteral, const char* beginningOfLineOfCode);

void auditConditionalStatementFormat(const struct LineOfCode* lineOfCode);







#endif
