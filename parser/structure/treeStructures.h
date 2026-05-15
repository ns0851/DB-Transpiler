#ifndef TREE_STRUCTURES_H
#define TREE_STRUCTURES_H

typedef struct TreeNode {
    char *value;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

extern TreeNode *root;

#endif