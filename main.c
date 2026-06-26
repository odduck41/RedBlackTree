#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef int NodeType;

int less(const NodeType a, const NodeType b) {
    return a < b;
}

int eq(const NodeType a, const NodeType b) {
    return a == b;
}

int bigger(const NodeType a, const NodeType b) {
    return a > b;
}

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

__attribute__((always_inline))
inline Node* granny(const Node* const node) {
    if (node == NULL) return NULL;
    if (node->parent == NULL) return NULL;
    return node->parent->parent;
}

__attribute__((always_inline))
inline Node* uncle(const Node* const node) {
    if (node == NULL) return NULL;
    if (node->parent == NULL) return NULL;
    if (granny(node) == NULL) return NULL;
    if (granny(node)->left == node->parent) {
        return granny(node)->right;
    }
    return granny(node)->left;
}

#define getColor(node) (node==NULL?(black):(node->color))
#define setColor(node, col) ((node==NULL)?(node->color):(node->color=col))

__attribute__((always_inline))
inline void case1(RBTree*, Node*);

__attribute__((always_inline))
inline void case2(RBTree*, Node*);

void case3(RBTree*, Node*);

__attribute__((always_inline))
inline void case4(RBTree*, Node*);

__attribute__((always_inline))
inline void case5(RBTree*, const Node*);

__attribute__((always_inline))
inline void case1(RBTree* tree, Node* node) {
    if (node->parent == NULL) {
        node->color = black;
        return;
    }
    case2(tree, node);
}

__attribute__((always_inline))
inline void case2(RBTree* tree, Node* node) {
    if (getColor(node->parent) == black) {
        return;
    }
    case3(tree, node);
}

void case3(RBTree* tree, Node* node) {
    if (getColor(uncle(node)) == red) {
        setColor(node, red);
        setColor(granny(node), red);
        setColor(uncle(node), black);
        setColor(node->parent, black);
        case1(tree, granny(node));
        return;
    }
    case4(tree, node);
}

__attribute__((always_inline))
inline void case4(RBTree* tree, Node* node) {
    if (granny(node) == NULL) __builtin_unreachable();

    if (granny(node)->left == node->parent && node->parent->right == node) {
        leftRotate(tree, node);
        node = node->left;
    } else if (granny(node)->right == node->parent && node->parent->left == node) {
        rightRotate(tree, node);
        node = node->right;
    }
    case5(tree, node);
}

__attribute__((always_inline))
inline void case5(RBTree* tree, const Node* node) {
    if (granny(node) == NULL) __builtin_unreachable();

    setColor(node->parent, black);
    setColor(granny(node), red);
    if (granny(node)->left == node->parent) {
        rightRotate(tree, node->parent);
    } else {
        leftRotate(tree, node->parent);
    }
    case1(tree, node->parent);
}

__attribute__((always_inline))
inline void fixup(RBTree* tree, Node* node) {
    case1(tree, node);
}

Node* insert(RBTree* tree, const NodeType val) {
    Node* node = malloc(sizeof(Node));
    node->value = val;
    node->color = red;
    node->parent = node->left = node->right = NULL;

    if (tree->root == NULL) {
        tree->root = node;
        fixup(tree, node);
        return node;
    }

    Node* parent = tree->root;
    Node* me = NULL;

    do {
        if (parent->value == val) {
            me = parent;
        } else if (parent->value < val) {
            me = parent->right;
        } else {
            me = parent->left;
        }
    } while (me != NULL && me != parent && (parent = me, 1));

    if (parent->value == val) {
        free(node);
        return parent;
    }

    if (parent->value < val) {
        parent->right = node;
    } else {
        parent->left = node;
    }
    node->parent = parent;

    fixup(tree, node);

    return node;
}

const Node* find(const RBTree* const tree, const NodeType key) {
    const Node* me = tree->root;
    do {
        if (me->value < key) me = me->right;
        if (me->value > key) me = me->left;
    } while (me != NULL && me->value != key);

    return me;
}



int main() {
    RBTree t;
    t.root = NULL;
    const clock_t begin = clock();

    for (int i = 0; i < 100000000; ++i) {
        insert(&t, i);
    }

    const clock_t end = clock();
    const double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("%lf", time_spent);
}