#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#include "lexer/lexer.h"
#include "parser/parser.h"
#include "parser/structure/treeStructures.h"
#include "generator/generator.h"

void free_lexems(Node *head) {
    Node *temp;
    while(head != NULL) {
        temp = head;
        head = head->next;
        free(temp->data.word);
        free(temp);
    }
}

void free_tree(TreeNode *node) {
    if(node == NULL) return;

    free_tree(node->left);
    free_tree(node->right);

    free(node->value);
    free(node);
}

void reset_query() {

    for(int i = 0; i < query.projection_count; i++) {
        free(query.projections[i]);
    }
    free(query.projections);

    query.projections = NULL;
    query.projection_count = 0;

    query.table_name = NULL;
    query.condition = NULL;
    free_tree(root);
    free_lexems(head);

    head = NULL;
    current = NULL;
    tail = NULL;
    root = NULL;
}

int main() {

    char input[1000];

    while(1) {  
        printf("\n>> ");
        fgets(input, sizeof(input), stdin);

        if(strcmp(input, "exit\n") == 0) {
            break;
        }
        bool sql = strstr(input, "-sql") != NULL;
        bool mongo = strstr(input, "-mongo") != NULL;

        char *flag = strstr(input, "-");

        if(flag != NULL) {
            *flag = '\0';
        }

        create_lexems(input);
        start_parser();

        if(sql) {
            generator("sql");
            printf("\n");
        }
        if(mongo) {
            generator("mongo");
            printf("\n");
        }
        if(!sql && !mongo) {
            generator("both");
            printf("\n");
        }

        reset_query();
    }

    return 0;
}