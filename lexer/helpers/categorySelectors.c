#include "categorySelectors.h"
#include "../structs/structures.h"

bool isNum(char a) {
    return a >= '0' && a <= '9';
}

bool isOperator(char a) {
    return a == '+' || a == '>' || a == '<' ||
           a == '=' || a == '!' || a == '|' || a == '&';
}

bool isString(char a) {
    return a == '"' || a == '\'';
}

bool isWord(char a) {
    return (a >= 'a' && a <= 'z') ||
           (a >= 'A' && a <= 'Z') ||
           a == '_';
}

bool isSpace(char a) {
    return (a == ' ') || (a == '\t') || (a == '\n');
}

bool isComma(char a) {
    return a == ',';
}

TokenType getFirstCategory(int i, char *str) {
    if(isNum(str[i])) return TOKEN_NUMBER;
    if(isOperator(str[i])) return TOKEN_OPERATOR;
    if(isString(str[i])) return TOKEN_STRING;
    if(isWord(str[i])) return TOKEN_IDENTIFIER;
    if(isString(str[i])) return TOKEN_STRING;
    if(isComma(str[i])) return TOKEN_COMMA;
    if(isSpace(str[i])) return TOKEN_SPACE;

    return TOKEN_UNKNOWN;
}