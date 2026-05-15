#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "../structs/structures.h"

extern Node *head;
extern Node *tail;
extern Node *current;

extern char *tokenNames[];


void insert_to_symbol_table(
    int initial,
    int length,
    char *str,
    TokenType type
);

#endif