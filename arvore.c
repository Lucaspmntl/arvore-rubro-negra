#include <stdio.h>
#include <stdlib.h>

#define RED 0
#define BLACK 1

typedef struct Node {
    int data;
    int color;
    struct Node *parent;
    struct Node *left;
    struct Node *right;
} Node;

Node *root;
Node *TNULL;

void initializeTNULL() {
    TNULL = (Node *)malloc(sizeof(Node));
    TNULL->color = BLACK;
    TNULL->left = NULL;
    TNULL->right = NULL;
}

Node* createNode(int data) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->data = data;
    node->parent = NULL;
    node->left = TNULL;
    node->right = TNULL;
    node->color = RED;
    return node;
}

