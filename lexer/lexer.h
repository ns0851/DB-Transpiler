#ifndef LEXER_H
#define LEXER_H

#include "structs/structures.h"

Result getNext(int i, char *str, int len);
void createLexems();

#endif