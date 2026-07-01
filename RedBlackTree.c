//
// Created by Lev Dymchenko on 01.07.2026.
//

#include "RedBlackTree.h"
#include <stdlib.h>
#include <string.h>

// NOLINTBEGIN(modernize-use-nullptr)

inline int less(const ValueType a, const ValueType b) {
    return a.key < b.key;
}

inline int eq(const ValueType a, const ValueType b) {
    return a.key == b.key;
}

inline int bigger(const ValueType a, const ValueType b) {
    return a.key > b.key;
}

inline Node * create(const ValueType v) {
    Node* p = malloc(sizeof(Node));
    p->left = p->right = p->parent = NULL;
    p->color = red;

    p->vertex.key = v.key;
    p->vertex.value = malloc(strlen(v.value) * sizeof(char));
    strcpy(p->vertex.value, v.value);
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

inline Color getColor(const Node * const node) {
    if (node == NULL) return black;
    return node->color;
}

inline Color setColor(Node * node, const Color color) {
    if (node == NULL) return black;
    return (node->color = color);
}

inline Node * grandparent(const Node * node) {
    if (node->parent == NULL) return NULL;
    return node->parent->parent;
}

inline Node * uncle(const Node * node) {
    if (node->parent == NULL) return NULL;
    if (node->parent->parent == NULL) return NULL;
    if (node->parent->parent->left == node->parent) return node->parent->parent->right;
    return node->parent->parent->left;
}

inline Node * sibling(const Node * node) {
    if (node->parent == NULL) return NULL;
    if (node->parent->left == node) return node->parent->right;
    return node->parent->left;
}

inline Node * insert(RBTree * rb_tree, const ValueType value) {
    Node * x = rb_tree->root;
    Node * xp = NULL;
    while (x != NULL) {
        xp = x;
        if (less(x->vertex, value)) x = x->right;
        else x = x->left;
    }

    Node * node = create(value);

    node->parent = xp;
    if (xp == NULL) {
        rb_tree->root = node;
    } else {
        if (less(value, xp->vertex)) xp->left = node;
        else xp->right = node;
    }

    insertFixup(rb_tree, node);

    return node;
}

inline void insertFixup(RBTree * rb_tree, Node * node) {
    while (getColor(node->parent) == red) {
        Node * u = uncle(node);
        Node * g = grandparent(node);
        if (g == NULL) __builtin_unreachable();

        if (getColor(u) == red) {
            setColor(node->parent, black);
            setColor(u, black);
            setColor(g, red);
            node = g;
        } else {
            if (node->parent == g->left) {
                if (node == node->parent->right) {
                    node = node->parent;
                    Node * now = leftRotate(node);
                    if (now->parent == NULL) rb_tree->root = now;
                }
                setColor(node->parent, black);
                setColor(g, red);
                Node * now = rightRotate(g);
                if (now->parent == NULL) rb_tree->root = now;
            } else {
                if (node->parent == g->right) {
                    if (node == node->parent->right) {
                        node = node->parent;
                        Node * now = rightRotate(node);
                        if (now->parent == NULL) rb_tree->root = now;
                    }
                    setColor(node->parent, black);
                    setColor(g, red);
                    Node * now = leftRotate(g);
                    if (now->parent == NULL) rb_tree->root = now;
                }
            }
        }
    }
}


Node * find(const RBTree * const tree, const ValueType val) {
    Node * x = tree->root;
    while (x != NULL && !eq(x->vertex, val)) {
        if (less(x->vertex, val)) x = x->right;
        else x = x->left;
    }

    return x;
}

// NOLINTEND(modernize-use-nullptr)