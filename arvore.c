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
Node *NULL_NODE;


void initializeNullNode() {
    NULL_NODE = (Node *)malloc(sizeof(Node));
    NULL_NODE->color = BLACK;
    NULL_NODE->left = NULL;
    NULL_NODE->right = NULL;
}


Node* createNode(int data) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->data = data;
    node->parent = NULL;
    node->left = NULL_NODE;
    node->right = NULL_NODE;
    node->color = RED;
    return node;
}


void leftRotate(Node *x) {
    Node *y = x->right;
    x->right = y->left;
    if (y->left != NULL_NODE) {
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == NULL) {
        root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}


void rightRotate(Node *x) {
    Node *y = x->left;
    x->left = y->right;
    if (y->right != NULL_NODE) {
        y->right->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == NULL) {
        root = y;
    } else if (x == x->parent->right) {
        x->parent->right = y;
    } else {
        x->parent->left = y;
    }
    y->right = x;
    x->parent = y;
}


void insertFixup(Node *k) {
    Node *u;
    while (k->parent->color == RED) {
        if (k->parent == k->parent->parent->right) {
            u = k->parent->parent->left;
            if (u->color == RED) {
                u->color = BLACK;
                k->parent->color = BLACK;
                k->parent->parent->color = RED;
                k = k->parent->parent;
            } else {
                if (k == k->parent->left) {
                    k = k->parent;
                    rightRotate(k);
                }
                k->parent->color = BLACK;
                k->parent->parent->color = RED;
                leftRotate(k->parent->parent);
            }
        } else {
            u = k->parent->parent->right;
            if (u->color == RED) {
                u->color = BLACK;
                k->parent->color = BLACK;
                k->parent->parent->color = RED;
                k = k->parent->parent;
            } else {
                if (k == k->parent->right) {
                    k = k->parent;
                    leftRotate(k);
                }
                k->parent->color = BLACK;
                k->parent->parent->color = RED;
                rightRotate(k->parent->parent);
            }
        }
        if (k == root) {
            break;
        }
    }
    root->color = BLACK;
}


void insert(int data) {
    Node *node = createNode(data);
    Node *y = NULL;
    Node *x = root;

    while (x != NULL_NODE) {
        y = x;
        if (node->data < x->data) {
            x = x->left;
        } else {
            x = x->right;
        }
    }

    node->parent = y;
    if (y == NULL) {
        root = node;
    } else if (node->data < y->data) {
        y->left = node;
    } else {
        y->right = node;
    }

    if (node->parent == NULL) {
        node->color = BLACK;
        return;
    }

    if (node->parent->parent == NULL) {
        return;
    }

    insertFixup(node);
}


void rbTransplant(Node *u, Node *v) {
    if (u->parent == NULL) {
        root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    v->parent = u->parent;
}


Node* minimum(Node *node) {
    while (node->left != NULL_NODE) {
        node = node->left;
    }
    return node;
}


void deleteFixup(Node *x) {
    Node *s;
    while (x != root && x->color == BLACK) {
        if (x == x->parent->left) {
            s = x->parent->right;
            if (s->color == RED) {
                s->color = BLACK;
                x->parent->color = RED;
                leftRotate(x->parent);
                s = x->parent->right;
            }
            if (s->left->color == BLACK && s->right->color == BLACK) {
                s->color = RED;
                x = x->parent;
            } else {
                if (s->right->color == BLACK) {
                    s->left->color = BLACK;
                    s->color = RED;
                    rightRotate(s);
                    s = x->parent->right;
                }
                s->color = x->parent->color;
                x->parent->color = BLACK;
                s->right->color = BLACK;
                leftRotate(x->parent);
                x = root;
            }
        } else {
            s = x->parent->left;
            if (s->color == RED) {
                s->color = BLACK;
                x->parent->color = RED;
                rightRotate(x->parent);
                s = x->parent->left;
            }
            if (s->right->color == BLACK && s->left->color == BLACK) {
                s->color = RED;
                x = x->parent;
            } else {
                if (s->left->color == BLACK) {
                    s->right->color = BLACK;
                    s->color = RED;
                    leftRotate(s);
                    s = x->parent->left;
                }
                s->color = x->parent->color;
                x->parent->color = BLACK;
                s->left->color = BLACK;
                rightRotate(x->parent);
                x = root;
            }
        }
    }
    x->color = BLACK;
}


void deleteNode(int key) {
    Node *z = NULL_NODE;
    Node *x, *y;
    Node *node = root;

    while (node != NULL_NODE) {
        if (node->data == key) {
            z = node;
        }
        if (node->data <= key) {
            node = node->right;
        } else {
            node = node->left;
        }
    }

    if (z == NULL_NODE) {
        printf("Value not found.\n");
        return;
    }

    y = z;
    int y_original_color = y->color;
    if (z->left == NULL_NODE) {
        x = z->right;
        rbTransplant(z, z->right);
    } else if (z->right == NULL_NODE) {
        x = z->left;
        rbTransplant(z, z->left);
    } else {
        y = minimum(z->right);
        y_original_color = y->color;
        x = y->right;
        if (y->parent == z) {
            x->parent = y;
        } else {
            rbTransplant(y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        rbTransplant(z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }
    if (y_original_color == BLACK) {
        deleteFixup(x);
    }
    free(z);
}


void search(int data) {
    Node *current = root;
    while (current != NULL_NODE) {
        if (data == current->data) {
            printf("Found: %d (Color: %s)\n", data, current->color == RED ? "RED" : "BLACK");
            return;
        }
        if (data < current->data) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    printf("Not Found.\n");
}


void preorder(Node *node) {
    if (node != NULL_NODE) {
        printf("%d (%s) ", node->data, node->color == RED ? "R" : "B");
        preorder(node->left);
        preorder(node->right);
    }
}


void inorder(Node *node) {
    if (node != NULL_NODE) {
        inorder(node->left);
        printf("%d (%s) ", node->data, node->color == RED ? "R" : "B");
        inorder(node->right);
    }
}


void postorder(Node *node) {
    if (node != NULL_NODE) {
        postorder(node->left);
        postorder(node->right);
        printf("%d (%s) ", node->data, node->color == RED ? "R" : "B");
    }
}

int main() {
    initializeNullNode();
    root = NULL_NODE;
    int choice, val, travChoice;

    while (1) {
        printf("\n=== RED BLACK TREE ===\n");
        printf("1. Insert\n2. Delete\n3. Search\n4. Display (Traversals)\n5. Exit\nChoice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Value: ");
                scanf("%d", &val);
                insert(val);
                break;
            case 2:
                printf("Value: ");
                scanf("%d", &val);
                deleteNode(val);
                break;
            case 3:
                printf("Value: ");
                scanf("%d", &val);
                search(val);
                break;
            case 4:
                printf("\nSelect Traversal Type:\n");
                printf("1. Pre-Order (Root-Left-Right)\n");
                printf("2. In-Order  (Left-Root-Right)\n");
                printf("3. Post-Order (Left-Right-Root)\n");
                printf("Option: ");
                scanf("%d", &travChoice);
                
                printf("\nResult: ");
                switch(travChoice) {
                    case 1: preorder(root); break;
                    case 2: inorder(root); break;
                    case 3: postorder(root); break;
                    default: printf("Invalid traversal option.");
                }
                printf("\n");
                break;
            case 5:
                return 0;
            default:
                printf("Invalid.\n");
        }
    }
}