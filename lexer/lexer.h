#ifndef LEXER_H
#define LEXER_H

#include "structs/structures.h"

Result get_next(int i, char *str, int len);
void create_lexems();

#endif