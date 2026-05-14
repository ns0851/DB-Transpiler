#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "../structs/structures.h"

extern symbolTable st[100];
extern int symbolCount;

extern char *tokenNames[];



void insertToSymbolTable(
    int initial,
    int length,
    char *str,
    TokenType type
);

#endif