//
// Created by Lev Dymchenko on 01.07.2026.
//

#ifndef REDBLACKTREE_H
#define REDBLACKTREE_H

typedef struct ValueType {
    int key;
    char* value;
} ValueType;

int less(ValueType, ValueType);
int eq(ValueType, ValueType);
int bigger(ValueType, ValueType);

typedef enum Color {
    black = 0,
    red = 1
} Color;

typedef struct Node Node;
typedef struct Node {
    Node * parent;
    ValueType vertex;
    Color color;
    Node * left;
    Node * right;
} Node;

Node * create(int, const char *);
void destroy(Node *);

Node * leftRotate(Node *);
Node * rightRotate(Node *);

Color getColor(const Node *);
Color setColor(Node *, Color);

Node * grandparent(const Node *);
Node * uncle(const Node *);

Node * sibling(const Node *);

typedef struct {
    Node * root;
} RBTree;

void insertFixup(RBTree *, Node*);
Node * insert(RBTree *, ValueType);

Node * find(RBTree *, ValueType);

void deleteFixup(RBTree *, Node *);
void delete(RBTree *, ValueType);



#endif //REDBLACKTREE_H
