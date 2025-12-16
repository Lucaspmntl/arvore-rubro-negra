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
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->data = data;
    newNode->parent = NULL;
    newNode->left = NULL_NODE;
    newNode->right = NULL_NODE;
    newNode->color = RED;
    return newNode;
}


void leftRotate(Node *nodeToRotate) {
    Node *rightChild = nodeToRotate->right;
    nodeToRotate->right = rightChild->left;

    if (rightChild->left != NULL_NODE) {
        rightChild->left->parent = nodeToRotate;
    }

    rightChild->parent = nodeToRotate->parent;

    if (nodeToRotate->parent == NULL) {
        root = rightChild;
    } else if (nodeToRotate == nodeToRotate->parent->left) {
        nodeToRotate->parent->left = rightChild;
    } else {
        nodeToRotate->parent->right = rightChild;
    }

    rightChild->left = nodeToRotate;
    nodeToRotate->parent = rightChild;
}


void rightRotate(Node *nodeToRotate) {
    Node *leftChild = nodeToRotate->left;
    nodeToRotate->left = leftChild->right;

    if (leftChild->right != NULL_NODE) {
        leftChild->right->parent = nodeToRotate;
    }

    leftChild->parent = nodeToRotate->parent;

    if (nodeToRotate->parent == NULL) {
        root = leftChild;
    } else if (nodeToRotate == nodeToRotate->parent->right) {
        nodeToRotate->parent->right = leftChild;
    } else {
        nodeToRotate->parent->left = leftChild;
    }

    leftChild->right = nodeToRotate;
    nodeToRotate->parent = leftChild;
}


void insertFixup(Node *fixingNode) {
    Node *uncle;

    while (fixingNode->parent->color == RED) {

        if (fixingNode->parent == fixingNode->parent->parent->right) {
            uncle = fixingNode->parent->parent->left;
            
            if (uncle->color == RED) {
                uncle->color = BLACK;
                fixingNode->parent->color = BLACK;
                fixingNode->parent->parent->color = RED;
                fixingNode = fixingNode->parent->parent;
            
            } else {
                if (fixingNode == fixingNode->parent->left) {
                    fixingNode = fixingNode->parent;
                    rightRotate(fixingNode);
                }
                fixingNode->parent->color = BLACK;
                fixingNode->parent->parent->color = RED;
                leftRotate(fixingNode->parent->parent);
            }
        
        } else {
            uncle = fixingNode->parent->parent->right;
            
            if (uncle->color == RED) {
                uncle->color = BLACK;
                fixingNode->parent->color = BLACK;
                fixingNode->parent->parent->color = RED;
                fixingNode = fixingNode->parent->parent;
            
            } else {
                if (fixingNode == fixingNode->parent->right) {
                    fixingNode = fixingNode->parent;
                    leftRotate(fixingNode);
                }
                fixingNode->parent->color = BLACK;
                fixingNode->parent->parent->color = RED;
                rightRotate(fixingNode->parent->parent);
            }
        }
        
        if (fixingNode == root) {
            break;
        }
    }
    root->color = BLACK;
}


void insert(int data) {
    Node *newNode = createNode(data);
    Node *parent = NULL;
    Node *current = root;

    while (current != NULL_NODE) {
        parent = current;
        if (newNode->data < current->data) {
            current = current->left;
        
        } else {
            current = current->right;
        }
    }

    newNode->parent = parent;
    if (parent == NULL) {
        root = newNode;

    } else if (newNode->data < parent->data) {
        parent->left = newNode;

    } else {
        parent->right = newNode;
    }

    if (newNode->parent == NULL) {
        newNode->color = BLACK;
        return;
    }

    if (newNode->parent->parent == NULL) {
        return;
    }

    insertFixup(newNode);
}


void rbTransplant(Node *oldNode, Node *newNode) {
    if (oldNode->parent == NULL) {
        root = newNode;

    } else if (oldNode == oldNode->parent->left) {
        oldNode->parent->left = newNode;

    } else {
        oldNode->parent->right = newNode;
    }

    newNode->parent = oldNode->parent;
}


Node* minimum(Node *node) {
    while (node->left != NULL_NODE) {
        node = node->left;
    }
    return node;
}


void deleteFixup(Node *doubleBlackNode) {
    Node *sibling;
    
    while (doubleBlackNode != root && doubleBlackNode->color == BLACK) {
        if (doubleBlackNode == doubleBlackNode->parent->left) {
            sibling = doubleBlackNode->parent->right;
            
            if (sibling->color == RED) {
                sibling->color = BLACK;
                doubleBlackNode->parent->color = RED;
                leftRotate(doubleBlackNode->parent);
                sibling = doubleBlackNode->parent->right;
            }
            
            if (sibling->left->color == BLACK && sibling->right->color == BLACK) {
                sibling->color = RED;
                doubleBlackNode = doubleBlackNode->parent;
            
            } else {
                if (sibling->right->color == BLACK) {
                    sibling->left->color = BLACK;
                    sibling->color = RED;
                    rightRotate(sibling);
                    sibling = doubleBlackNode->parent->right;
                }
                sibling->color = doubleBlackNode->parent->color;
                doubleBlackNode->parent->color = BLACK;
                sibling->right->color = BLACK;
                leftRotate(doubleBlackNode->parent);
                doubleBlackNode = root;
            }
        
        } else {
            sibling = doubleBlackNode->parent->left;
            if (sibling->color == RED) {
                sibling->color = BLACK;
                doubleBlackNode->parent->color = RED;
                rightRotate(doubleBlackNode->parent);
                sibling = doubleBlackNode->parent->left;
            }
            
            if (sibling->right->color == BLACK && sibling->left->color == BLACK) {
                sibling->color = RED;
                doubleBlackNode = doubleBlackNode->parent;
            
            } else {
                if (sibling->left->color == BLACK) {
                    sibling->right->color = BLACK;
                    sibling->color = RED;
                    leftRotate(sibling);
                    sibling = doubleBlackNode->parent->left;
                }
                sibling->color = doubleBlackNode->parent->color;
                doubleBlackNode->parent->color = BLACK;
                sibling->left->color = BLACK;
                rightRotate(doubleBlackNode->parent);
                doubleBlackNode = root;
            }
        }
    }
    doubleBlackNode->color = BLACK;
}


void deleteNode(int key) {
    Node *nodeToDelete = NULL_NODE;
    Node *replacementNode, *movedNode;
    Node *current = root;

    while (current != NULL_NODE) {
        if (current->data == key) {
            nodeToDelete = current;
        }
        if (current->data <= key) {
            current = current->right;
        } else {
            current = current->left;
        }
    }

    if (nodeToDelete == NULL_NODE) {
        printf("Value not found.\n");
        return;
    }

    movedNode = nodeToDelete;
    int movedNodeOriginalColor = movedNode->color;

    if (nodeToDelete->left == NULL_NODE) {
        replacementNode = nodeToDelete->right;
        rbTransplant(nodeToDelete, nodeToDelete->right);
    
    } else if (nodeToDelete->right == NULL_NODE) {
        replacementNode = nodeToDelete->left;
        rbTransplant(nodeToDelete, nodeToDelete->left);
    
    } else {
        movedNode = minimum(nodeToDelete->right);
        movedNodeOriginalColor = movedNode->color;
        replacementNode = movedNode->right;

        if (movedNode->parent == nodeToDelete) {
            replacementNode->parent = movedNode;
        } else {
            rbTransplant(movedNode, movedNode->right);
            movedNode->right = nodeToDelete->right;
            movedNode->right->parent = movedNode;
        }

        rbTransplant(nodeToDelete, movedNode);
        movedNode->left = nodeToDelete->left;
        movedNode->left->parent = movedNode;
        movedNode->color = nodeToDelete->color;
    }

    if (movedNodeOriginalColor == BLACK) {
        deleteFixup(replacementNode);
    }
    free(nodeToDelete);
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
    int choice, value, traversalChoice;

    while (1) {
        printf("\n=== RED BLACK TREE ===\n");
        printf("1. Insert\n2. Delete\n3. Search\n4. Display (Traversals)\n5. Exit\nChoice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Value: ");
                scanf("%d", &value);
                insert(value);
                break;

            case 2:
                printf("Value: ");
                scanf("%d", &value);
                deleteNode(value);
                break;

            case 3:
                printf("Value: ");
                scanf("%d", &value);
                search(value);
                break;

            case 4:
                printf("\nSelect Traversal Type:\n");
                printf("1. Pre-Order (Root-Left-Right)\n");
                printf("2. In-Order  (Left-Root-Right)\n");
                printf("3. Post-Order (Left-Right-Root)\n");
                printf("Option: ");
                scanf("%d", &traversalChoice);
                
                printf("\nResult: ");
                switch(traversalChoice) {
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