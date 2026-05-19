#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "generator.h"
#include "../parser/structure/treeStructures.h"
#include "../parser/parser.h"
#include "../lexer/lexer.h"

Pair sql[] = {
    {"get", "SELECT"},
    {"from", "FROM"},
    {"when", "WHERE"},
    {"and", "AND"},
    {"or", "OR"}
};

void print_tree_sql(TreeNode *node) {
    if(node == NULL) return;
    print_tree_sql(node->left);
    printf("%s ", node->value);
    print_tree_sql(node->right);
}

void print_mongo(TreeNode *node) {

    if(node == NULL) return;
    if(strcmp(node->value, "and") == 0) {
        printf("$and: [ ");
        print_mongo(node->left);
        printf(", ");
        print_mongo(node->right);
        printf(" ]");
        return;
    }
    
    if(strcmp(node->value, "=") == 0) {
        printf("{ %s: ", node->left->value);
        printf("%s }", node->right->value);
        return;
    }

    if(strcmp(node->value, ">") == 0) {
        printf("{ %s: { $gt: %s } }",
            node->left->value,
            node->right->value);
            return;
        }
        
    if(strcmp(node->value, "<") == 0) {
        printf("{ %s: { $lt: %s } }",
            node->left->value,
            node->right->value);
            return;
        }
    if(strcmp(node->value, ">=") == 0) {
        printf("{ %s: { $gte: %s } }",
        node->left->value,
        node->right->value);
        return;
    }
        
        if(strcmp(node->value, "<=") == 0) {
        printf("{ %s: { $lte: %s } }",
        node->left->value,
        node->right->value);
        return;
    }
}

bool isKeyword(char *str) {
    for(int i = 0; i < 7; i++) {
        if(strcmp(str, keywords[i]) == 0) {
            return true;
        }
    }
    return false;
}

char* lookup(char *key, Pair *dict, int size) {
    for(int i = 0; i < size; i++) {
        if(strcmp(dict[i].key, key) == 0) {
            return dict[i].value;
        }
    }

    return NULL;
}

void generate_sql() {
    printf("Generating SQL\n");
    for(int i = 0; i < query.projection_count-1; i++) {
        if(isKeyword(convert_to_lower(query.projections[i]))) {
            printf("%s ", lookup(query.projections[i], sql, 5));
        } else if(isKeyword(convert_to_lower(query.projections[i+1]))) {
            printf("%s ", query.projections[i]);
        } else {
            printf("%s, ", query.projections[i]);
        }
    }
    printf("%s ", query.table_name);
    printf("%s ", lookup(convert_to_lower(query.projections[query.projection_count-1]), sql, 5));

    print_tree_sql(query.condition);
}

void generate_mongo() {
    printf("Generating MongoDB\n");
    printf("db.%s.find(", query.table_name);

    if(query.condition != NULL) {
        print_mongo(query.condition);
    } else {
        printf("{}");
    }

    bool has_projection = false;

    for(int i = 0; i < query.projection_count; i++) {
        if(!isKeyword(convert_to_lower(query.projections[i])) &&
           strcmp(query.projections[i], "*") != 0) {
            has_projection = true;
            break;
        }
    }

    if(has_projection) {
        printf(", { ");
        bool first = true;

        for(int i = 0; i < query.projection_count; i++) {
            if(!isKeyword(convert_to_lower(query.projections[i])) && strcmp(query.projections[i], "*") != 0) {
                if(!first) {
                    printf(", ");
                }
                printf("%s: 1", query.projections[i]);
                first = false;
            }
        }
        printf(" }");
    }
    printf(")");
}

void generator(char *type) {
    if(strcmp(type, "sql") == 0) {
        generate_sql();
    } else if(strcmp(type, "mongo") == 0) {
        generate_mongo();
    } else if(strcmp(type, "both") == 0) {
        generate_sql();
        printf("\n");
        generate_mongo();
    }
}