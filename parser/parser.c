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

QuerySkeleton query = {
    .projections = NULL,
    .projection_count = 0,
    .table_name = NULL,
    .condition = NULL
};

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

void advance() {
    if(current != NULL) {
        // printf("Consumed: %s\n", current->data.word);
        current = current->next;
    }
}

void add_projection(char *name) {
    char **temp = realloc(
        query.projections,
        sizeof(char*) * (query.projection_count + 1)
    );

    if(temp == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }

    query.projections = temp;
    query.projections[query.projection_count] = strdup(name);
    query.projection_count++;
}

void expect(TokenType type, char *value) {
    char temp[100];
    strcpy(temp, current->data.word);

    convert_to_lower(temp);
    if(current->data.type != type || strcmp(temp, value) != 0) {
        printf("Syntax Error: Expected '%s' found '%s'\n", value, current->data.word);
        exit(1);
    }
    add_projection(current->data.word);
    advance();
}



// void add_table_name(char *name) {
//     query.table_name = strdup(name);
// }

void parse_projection() {
    if(current->data.type == TOKEN_KEYWORD && strcmp(current->data.word, "*") == 0) {
        add_projection("*");
        advance();
    }
    
    else if(current->data.type == TOKEN_KEYWORD && strcmp(current->data.word, "everything") == 0) {
        add_projection("*");
        advance();
    }

    else {
        while(current != NULL) {
            if(current->data.type == TOKEN_IDENTIFIER) {
                add_projection(current->data.word);
                advance();
            } else {
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
        query.table_name = current->data.word;
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
    } else {
        printf("Expected identifier\n");
        exit(1);
    }

    if(current != NULL && current->data.type == TOKEN_OPERATOR) {
        node = createNode(current->data.word);
        node->left = leftnode;

        advance();
    } else {
        printf("Expected operator\n");
        exit(1);
    }

    if(current != NULL && (current->data.type == TOKEN_NUMBER || current->data.type == TOKEN_IDENTIFIER || current->data.type == TOKEN_STRING)) {
        rightnode = createNode(current->data.word);
        node->right = rightnode;

        advance();
    } else {
        printf("Expected value\n");
        exit(1);
    }

    return node;
}

TreeNode* parse_condition() {
    // printf("DEBUG: entered parse_condition\n");
    TreeNode *node = parse_subquery();
    // printf("DEBUG: subquery returned, node = %s\n", node == NULL ? "NULL" : node->value);

    if(current != NULL && current->data.type == TOKEN_KEYWORD && strcasecmp(current->data.word, "and") == 0) {
        TreeNode *astNode = malloc(sizeof(TreeNode));
        astNode->value = strdup("and");
        astNode->left = NULL;
        astNode->right = NULL;
        // printf("AST Root is: %s\n", astNode == NULL ? "NULL" : astNode->value); 
        advance();
        astNode->left = node;
        // printf("DEBUG: BEFORE returning astNode\n");
        astNode->right = parse_condition();
        // printf("DEBUG: returning astNode\n");
        return astNode;
    }

    // printf("DEBUG: returning normal node %s\n", node->value);
    return node;
}

void parse_query() {
    expect(TOKEN_KEYWORD, "get");
    parse_projection();
    expect(TOKEN_KEYWORD, "from");
    parse_source();
    if(current != NULL && current->data.type == TOKEN_KEYWORD && strcasecmp(current->data.word, "when") == 0) {
        // add_table_name(current->data.word);
        add_projection(current->data.word);
        advance();
        root = parse_condition();
        // printf("DEBUG: condition done, root = %s\n", root == NULL ? "NULL" : root->value);
    }
}

void start_parser() {
    current = head;
    if(current == NULL) {
        printf("No tokens found\n");
        return;
    }

    parse_query();
    query.condition = root;

    printf("Query parsed successfully!\n");
}

