#ifndef CATEGORY_SELECTORS_H
#define CATEGORY_SELECTORS_H

#include "../structs/structures.h"

#include <stdbool.h>

bool is_num(char a);
bool is_operator(char a);
bool is_string(char a);
bool is_word(char a);
bool is_space(char a);
bool is_string(char a);
bool is_comma(char a);
TokenType get_first_category(int i, char *str);

#endif