#include <stdio.h>
#include <stdlib.h>

typedef int NodeType;

typedef enum Color {
    red,
    black
} Color;

typedef struct Node Node;
typedef struct Node {
    Node* parent;
    Node* left;
    NodeType value;
    Color color;
    Node* right;
} Node;

typedef struct RBTree {
    Node* root;
} RBTree;

void leftRotate(RBTree* const tree, Node* const node) {
/*
        |               |
        A               B
      /   \           /   \
     Al    B   -->   A    Br
          / \       / \
         Bl  Br    Al  Bl
*/
    if (node == NULL) return;
    if (node->parent == NULL) return;

    node->parent->right = node->left;
    if (node->left != NULL)
        node->left->parent = node->parent;

    node->left = node->parent;
    node->parent = node->left->parent;

    if (node->parent != NULL) {
        if (node->parent->left == node->left) {
            node->parent->left = node;
        } else {
            node->parent->right = node;
        }
    } else {
        tree->root = node;
    }

    node->left->parent = node;
}

void rightRotate(RBTree* const tree, Node* const node) {
/*
        |               |
        A               B
      /   \           /   \
     B     Ar  -->   Bl    A
    / \                   / \
   Bl  Br                Br  Ar
*/
    if (node == NULL) return;
    if (node->parent == NULL) return;

    node->parent->left = node->right;
    if (node->right != NULL)
        node->right->parent = node->parent;

    node->right = node->parent;
    node->parent = node->right->parent;

    if (node->parent != NULL) {
        if (node->parent->left == node->right) {
            node->parent->left = node;
        } else {
            node->parent->right = node;
        }
    } else {
        tree->root = node;
    }

    node->right->parent = node;
}

int main() {

}