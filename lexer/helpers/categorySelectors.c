#include "categorySelectors.h"
#include "../structs/structures.h"

bool is_num(char a) {
    return a >= '0' && a <= '9';
}

bool is_operator(char a) {
    return a == '>' || a == '<' ||
           a == '=' || a == '!' || a == '|' || a == '&';
}

bool is_string(char a) {
    return a == '"' || a == '\'';
}

bool is_word(char a) {
    return (a >= 'a' && a <= 'z') ||
           (a >= 'A' && a <= 'Z') ||
           a == '_';
}

bool is_space(char a) {
    return (a == ' ') || (a == '\t') || (a == '\n');
}

bool is_comma(char a) {
    return a == ',';
}

TokenType get_first_category(int i, char *str) {
    if(is_num(str[i])) return TOKEN_NUMBER;
    if(is_operator(str[i])) return TOKEN_OPERATOR;
    if(is_string(str[i])) return TOKEN_STRING;
    if(is_word(str[i])) return TOKEN_IDENTIFIER;
    if(is_string(str[i])) return TOKEN_STRING;
    if(is_comma(str[i])) return TOKEN_COMMA;
    if(is_space(str[i])) return TOKEN_SPACE;

    return TOKEN_UNKNOWN;
}