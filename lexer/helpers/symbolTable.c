#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "symbolTable.h"

symbolTable st[100];
int symbolCount = 0;

char *tokenNames[] = {
    "KEYWORD",
    "IDENTIFIER",
    "NUMBER",
    "OPERATOR",
    "STRING",
    "COMMA",
    "SPACE",
    "UNKNOWN"
};

void insertToSymbolTable(int initial, int length, char *str, TokenType type) {
    char *word = malloc(length + 1);

    int k = 0;

    for(int i = initial; i < length; i++) {
        word[k++] = str[i];
    }

    word[k] = '\0';

    st[symbolCount].word = strdup(word);
    st[symbolCount].type = type;

    symbolCount++;

    free(word);
}