//
// Created by Lev Dymchenko on 01.07.2026.
//

#ifndef REDBLACKTREE_H
#define REDBLACKTREE_H

typedef struct ValueType {
    int key;
    char* value;
} ValueType;

bool less(ValueType, ValueType);
bool eq(ValueType, ValueType);
bool bigger(ValueType, ValueType);

typedef enum Color {
    black = 0,
    red = 1
} Color;

typedef struct Node Node;
typedef struct Node {
    Node * parent;
    ValueType vertex;
    Color color_;
    Node * left;
    Node * right;
} Node;

Node * create(int, const char *);
void destroy(Node *);

Node * leftRotate(Node *);
Node * rightRotate(Node *);

typedef struct {
    Node * root;
} RBTree;

#endif //REDBLACKTREE_H
