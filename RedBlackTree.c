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

inline void swapValues(Node * a, Node * b) {
    if (a == NULL || b == NULL) return;
    const ValueType vertex = a->vertex;
    a->vertex = b->vertex;
    b->vertex = vertex;
}

inline Node * insert(RBTree * tree, const ValueType val) {
    Node * x = tree->root;
    Node * xp = NULL;
    while (x != NULL) {
        xp = x;
        if (less(x->vertex, val)) x = x->right;
        else if (bigger(x->vertex, val)) x = x->left;
        else if (eq(x->vertex, val)) {
            if (tree->multiset) x = x->right;
            else return x;
        }
    }

    Node * node = create(val);

    node->parent = xp;
    if (xp == NULL) {
        tree->root = node;
    } else {
        if (less(val, xp->vertex)) xp->left = node;
        else xp->right = node;
    }

    insertFixup(tree, node);

    return node;
}

inline void insertFixup(RBTree * tree, Node * node) {
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
                    Node * rotate = leftRotate(node);
                    if (rotate->parent == NULL) tree->root = rotate;
                }
                setColor(node->parent, black);
                setColor(g, red);
                Node * rotate = rightRotate(g);
                if (rotate->parent == NULL) tree->root = rotate;
            } else {
                if (node->parent == g->right) {
                    if (node == node->parent->left) {
                        node = node->parent;
                        Node * rotate = rightRotate(node);
                        if (rotate->parent == NULL) tree->root = rotate;
                    }
                    setColor(node->parent, black);
                    setColor(g, red);
                    Node * rotate = leftRotate(g);
                    if (rotate->parent == NULL) tree->root = rotate;
                }
            }
        }
    }
    setColor(tree->root, black);
}


inline Node * find(const RBTree * const tree, const ValueType val) {
    Node * x = tree->root;
    while (x != NULL && !eq(x->vertex, val)) {
        if (less(x->vertex, val)) x = x->right;
        else x = x->left;
    }

    return x;
}

inline void deleteFixup(RBTree * tree, const Node * node) {
    while (node != tree->root && getColor(node) == black) {
        Node * s = sibling(node);
        if (node == node->parent->left) {
            if (getColor(s) == red) {
                setColor(s, black);
                setColor(node->parent, red);
                Node * rotate = leftRotate(node->parent);
                if (rotate->parent == NULL) tree->root = rotate;
                s = sibling(node);
            }
            if (getColor(s->left) == black && getColor(s->right) == black) {
                setColor(s, red);
                node = node->parent;
            } else {
                if (getColor(s->right) == black) {
                    setColor(s->left, black);
                    setColor(s, red);
                    Node * rotate = rightRotate(s);
                    if (rotate->parent == NULL) tree->root = rotate;
                    s = sibling(node);
                }
                setColor(s, getColor(node->parent));
                setColor(s->parent, black);
                setColor(s->right, black);
                Node * rotate = leftRotate(node->parent);
                if (rotate->parent == NULL) tree->root = rotate;
                node = tree->root;
            }
        } else {
            if (getColor(s) == red) {
                setColor(s, black);
                setColor(node->parent, red);
                Node * rotate = rightRotate(node->parent);
                if (rotate->parent == NULL) tree->root = rotate;
                s = sibling(node);
            }
            if (getColor(s->right) == black && getColor(s->left) == black) {
                setColor(s, red);
                node = node->parent;
            } else {
                if (getColor(s->left) == black) {
                    setColor(s->right, black);
                    setColor(s, red);
                    Node * rotate = leftRotate(s);
                    if (rotate->parent == NULL) tree->root = rotate;
                    s = sibling(node);
                }
                setColor(s, getColor(node->parent));
                setColor(s->parent, black);
                setColor(s->left, black);
                Node * rotate = rightRotate(node->parent);
                if (rotate->parent == NULL) tree->root = rotate;
                node = tree->root;
            }
        }
    }
}

inline void delete(RBTree * tree, const ValueType val) {
    Node * x = find(tree, val);

    if (x == NULL) return;

    Node * y = NULL;
    if (x->left != NULL) {
        y = x->left;
        while (y->right != NULL) y= y->right;
    } else if (x->right != NULL) {
        y = x->right;
        while (y->left != NULL) y = y->left;
    }

    swapValues(x, y);
    if (y != NULL)
        x = y;
    if (getColor(x) == black)
        deleteFixup(tree, x);

    if (x == NULL) __builtin_unreachable();
    if (x->parent != NULL && x->parent->left == x)
        x->parent->left = NULL;
    else if (x->parent != NULL && x->parent->right == x)
        x->parent->right = NULL;
    else
        tree->root = NULL;

    destroy(x);
}

void inorder(const Node * const v, void(*f)(const Node * const)) {
    if (v == NULL) return;

    inorder(v->left, f);

    if (f != NULL) f(v);

    inorder(v->right, f);
}

// NOLINTEND(modernize-use-nullptr)
