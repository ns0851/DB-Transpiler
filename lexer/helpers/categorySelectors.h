#ifndef CATEGORY_SELECTORS_H
#define CATEGORY_SELECTORS_H

#include "../structs/structures.h"

#include <stdbool.h>

bool isNum(char a);
bool isOperator(char a);
bool isString(char a);
bool isWord(char a);
bool isSpace(char a);
bool isString(char a);
bool isComma(char a);
TokenType getFirstCategory(int i, char *str);

#endif