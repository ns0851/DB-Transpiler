#ifndef TREE_STRUCTURES_H
#define TREE_STRUCTURES_H


typedef struct TreeNode {
    char *value;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

typedef struct QuerySkeleton {
    char** projections;    
    int projection_count;
    char* table_name;      
    TreeNode* condition;
} QuerySkeleton;


extern TreeNode *root;
extern QuerySkeleton query;

#endif