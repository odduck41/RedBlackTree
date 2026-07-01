//
// Created by Lev Dymchenko on 01.07.2026.
//

#include "RedBlackTree.h"
#include <stdlib.h>
#include <string.h>

inline int less(const ValueType a, const ValueType b) {
    return a.key < b.key;
}

inline int eq(const ValueType a, const ValueType b) {
    return a.key == b.key;
}

inline int bigger(const ValueType a, const ValueType b) {
    return a.key > b.key;
}

inline Node * create(const int key, const char* value) {
    Node* p = malloc(sizeof(Node));
    p->vertex.key = key;
    p->vertex.value = malloc(strlen(value) * sizeof(char));
    strcpy(p->vertex.value, value);
    return p;
}

inline void destroy(Node * val) {
    free(val->vertex.value);
    free(val);
}

inline Node * leftRotate(Node * me) {
/*
        |               |
        A               B
      /   \           /   \
     Al    B   -->   A    Br
          / \       / \
         Bl  Br    Al  Bl
*/
    Node * pivot = me->right;

    pivot->parent = me->parent;
    if (me->parent != NULL) {
        if (me->parent->left == me) {
            me->parent->left = pivot;
        } else {
            me->parent->right = pivot;
        }
    }

    me->right = pivot->left;
    if (pivot->left != NULL)
        pivot->left->parent = me;

    me->parent = pivot;
    pivot->left = me;

    return pivot;
}

inline Node * rightRotate(Node * me) {
/*
        |               |
        A               B
      /   \           /   \
     B     Ar  -->   Bl    A
    / \                   / \
   Bl  Br                Br  Ar
*/
    Node * pivot = me->left;

    pivot->parent = me->parent;
    if (me->parent != NULL) {
        if (me->parent->left == me) {
            pivot->parent->left = pivot;
        } else {
            me->parent->right = pivot;
        }
    }

    me->left = pivot->right;
    if (pivot->right != NULL)
        pivot->right->parent = me;

    me->parent = pivot;
    pivot->right = me;

    return pivot;
}

Color getColor(const Node * const node) {
    if (node == NULL) return black;
    return node->color;
}

Color setColor(Node * node, const Color color) {
    if (node == NULL) return black;
    return (node->color = color);
}

Node * grandparent(const Node * node) {
    if (node->parent == NULL) return NULL; // NOLINT
    return node->parent->parent;
}

Node * uncle(const Node * node) {
    if (node->parent == NULL) return NULL; //NOLINT
    if (node->parent->parent == NULL) return NULL; // NOLINT
    if (node->parent->parent->left == node->parent) return node->parent->parent->right;
    return node->parent->parent->left;
}

Node * sibling(const Node * node) {
    if (node->parent == NULL) return NULL; // NOLINT
    if (node->parent->left == node) return node->parent->right;
    return node->parent->left;
}
