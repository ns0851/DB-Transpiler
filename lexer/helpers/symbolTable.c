#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "symbolTable.h"

Node *head = NULL;
Node *tail = NULL;
Node *current = NULL;

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

void insert_to_symbol_table(int initial, int length, char *str, TokenType type) {
    char *word = malloc(length + 1);

    int k = 0;

    for(int i = initial; i < length; i++) {
        word[k++] = str[i];
    }

    word[k] = '\0';

    Node *newNode = malloc(sizeof(Node));
    newNode->data.word = strdup(word);
    newNode->data.type = type;
    newNode->next = NULL;
    if(head == NULL) {
        head = newNode;
        tail = newNode;
    } else {
        tail->next = newNode;
        tail = newNode;
    }


    free(word);
}