#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <strings.h>

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

TreeNode* createNode(char *value) {
    TreeNode *node = malloc(sizeof(TreeNode));

    node->value = strdup(value);
    node->left = NULL;
    node->right = NULL;

    return node;
}

void print_tree(TreeNode *node, int level, char *prefix) {

    if(node == NULL) return;

    for(int i = 0; i < level; i++) {
        printf("    ");
    }

    printf("%s%s\n", prefix, node->value);

    if(node->left != NULL || node->right != NULL) {

        print_tree(node->left, level + 1, "|-- ");
        print_tree(node->right, level + 1, "`-- ");
    }
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

    TreeNode *leftnode = NULL;
    TreeNode *node = NULL;
    TreeNode *rightnode = NULL;

    if(current != NULL && current->data.type == TOKEN_IDENTIFIER) {

        leftnode = createNode(current->data.word);
        advance();
    }
    else {
        printf("Expected identifier\n");
        exit(1);
    }

    if(current != NULL && current->data.type == TOKEN_OPERATOR) {

        node = createNode(current->data.word);
        node->left = leftnode;

        advance();
    }
    else {
        printf("Expected operator\n");
        exit(1);
    }

    if(current != NULL &&
       (current->data.type == TOKEN_NUMBER ||
        current->data.type == TOKEN_IDENTIFIER)) {

        rightnode = createNode(current->data.word);

        node->right = rightnode;

        advance();
    }
    else {
        printf("Expected value\n");
        exit(1);
    }

    return node;
}

TreeNode* parse_condition() {
    printf("DEBUG: entered parse_condition\n");
    TreeNode *node = parse_subquery();
    printf("DEBUG: subquery returned, node = %s\n", node == NULL ? "NULL" : node->value);

    if(current != NULL && current->data.type == TOKEN_KEYWORD && strcasecmp(current->data.word, "and") == 0) {
        TreeNode *astNode = malloc(sizeof(TreeNode));
        astNode->value = strdup("and");
        astNode->left = NULL;
        astNode->right = NULL;
        printf("AST Root is: %s\n", astNode == NULL ? "NULL" : astNode->value);
        advance();
        astNode->left = node;
        printf("DEBUG: BEFOREreturning astNode\n");
        astNode->right = parse_condition();
        printf("DEBUG: returning astNode\n");
        return astNode;
    }

    printf("DEBUG: returning normal node %s\n", node->value);
    return node;
}

void parse_query() {
    expect(TOKEN_KEYWORD, "get");
    parse_projection();
    expect(TOKEN_KEYWORD, "from");
    parse_source();
    if(current != NULL && current->data.type == TOKEN_KEYWORD && strcasecmp(current->data.word, "when") == 0) {
        advance();
        root = parse_condition();
        printf("DEBUG: condition done, root = %s\n", root == NULL ? "NULL" : root->value);
    }
}

void start() {
    current = head;
    if(current == NULL) {
        printf("No tokens found\n");
        return;
    }
    parse_query();
    printf("DEBUG: about to print tree, root = %s\n", root == NULL ? "NULL" : root->value);
    print_tree(root, 0, "");

    printf("Query parsed successfully!\n");
}

