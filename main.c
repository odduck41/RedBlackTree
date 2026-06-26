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



int main() {

}