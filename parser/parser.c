#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "parser.h"
#include "../lexer/helpers/symbolTable.h"
#include "../lexer/structs/structures.h"
#include "structure/treeStructures.h"

TreeNode *root = NULL;

char* convert_to_lower(char *str) {
    for(int i = 0; i < strlen(str); i++) {
        str[i] = tolower(str[i]);
    }
    return str;
}

void advance() {
    if(current != NULL) {
        printf("Consumed: %s\n", current->data.word);
        current = current->next;
    }
}


void expect(TokenType type, char *value) {
    char temp[100];
    strcpy(temp, current->data.word);

    convert_to_lower(temp);
    if(current->data.type != type || strcmp(temp, value) != 0) {
        printf("Syntax Error: Expected '%s' found '%s'\n", value, current->data.word);
        exit(1);
    }
    advance();
}

void parse_projection() {
    if(current->data.type == TOKEN_KEYWORD && strcmp(current->data.word, "*") == 0) {
        advance();
    } 
    if(current->data.type == TOKEN_KEYWORD && strcmp(current->data.word, "everything") == 0) {
        advance();
    } else {

        while(current!=NULL) {
            if(current->data.type == TOKEN_IDENTIFIER) {
                advance();
            }
            else {
                 printf("Expected identifier\n");
                exit(1);
            }

            if(current->data.type == TOKEN_COMMA) {
                advance();
            } else {
                break;
            }
        }
        
    }
}

void parse_source() {
    if(current->data.type == TOKEN_IDENTIFIER) {
        advance();
    } else {
        printf("Expected identifier\n");
        exit(1);
    }
}

TreeNode* parse_subquery() {
    TreeNode *leftnode;
    TreeNode *node;
    TreeNode *rightnode;

    if(current->data.type == TOKEN_IDENTIFIER) {
        leftnode = malloc(sizeof(TreeNode));
        leftnode->value = strdup(current->data.word);
        advance();
    }

    if(current->data.type == TOKEN_OPERATOR) {
        node = malloc(sizeof(TreeNode));
        node->value = strdup(current->data.word);
        node->left = leftnode;
        advance();
    }

    if(current->data.type == TOKEN_NUMBER || current->data.type == TOKEN_IDENTIFIER) {
        rightnode = malloc(sizeof(TreeNode));
        rightnode->value = strdup(current->data.word);
        node->right = rightnode;
        advance();
    }
    return node;
}

TreeNode* parse_condition() {
    struct TreeNode *node = parse_subquery();
    struct TreeNode *astRoot = malloc(sizeof(TreeNode));
    if(root == NULL) {
        root = astRoot; 
    }
    if(current->data.type == TOKEN_KEYWORD && (strcmp(current->data.word, "and") == 0 || strcmp(current->data.word, "AND") == 0)) {
        advance();
        astRoot->left = node;
        astRoot->right = parse_condition();
        return astRoot;
    } else {
        astRoot->left = node;
        astRoot->right = NULL;
        return node;
    }
}

void parse_query() {
    expect(TOKEN_KEYWORD, "get");
    parse_projection();
    expect(TOKEN_KEYWORD, "from");
    parse_source();
    if(current != NULL && current->data.type == TOKEN_KEYWORD && strcmp(current->data.word, "when") == 0) {
        advance();
        parse_condition();
    }
    parse_condition();
    
}

void start() {
    current = head;
    if(current == NULL) {
        printf("No tokens found\n");
        return;
    }
    parse_query();

    printf("Query parsed successfully!\n");
}

