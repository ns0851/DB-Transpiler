#ifndef STRUCTURES_H
#define STRUCTURES_H

typedef struct {
    int count;
    int line;
    char *error;
    char token;
} Result;

typedef enum {
    TOKEN_KEYWORD,
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_COMMA,
    TOKEN_SPACE,
    TOKEN_UNKNOWN
} TokenType;


typedef struct {
    char *word;
    TokenType type;
} symbolTable;


typedef struct Node {
    symbolTable data;
    struct Node *next;
} Node;

#endif