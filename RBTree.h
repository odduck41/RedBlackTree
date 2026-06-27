#ifndef RBTREE_RBTREE_H
#define RBTREE_RBTREE_H
#include <stdlib.h>

typedef struct {
    int key;
    char* value;
} NodeType;

int less(NodeType a, NodeType b);

int eq(NodeType a, NodeType b);

int bigger(NodeType a, NodeType b);

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

void* allocator(int, const char*);
void deallocator(void*);

typedef struct RBTree {
    Node* root;
} RBTree;

void leftRotate(RBTree* tree, Node* node);

void rightRotate(RBTree* tree, Node* node);

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
inline void insert_case1(RBTree*, Node*);

__attribute__((always_inline))
inline void insert_case2(RBTree*, Node*);

void insert_case3(RBTree*, Node*);

__attribute__((always_inline))
inline void insert_case4(RBTree*, const Node*);

__attribute__((always_inline))
inline void insert_case5(RBTree*, const Node*);

__attribute__((always_inline))
inline void insert_case1(RBTree* tree, Node* node) {
    if (node->parent == NULL) {
        tree->root = node;
        node->color = black;
        return;
    }
    insert_case2(tree, node);
}

__attribute__((always_inline))
inline void insert_case2(RBTree* tree, Node* node) {
    if (getColor(node->parent) == black) {
        return;
    }
    insert_case3(tree, node);
}

__attribute__((always_inline))
inline void insert_case4(RBTree* tree, const Node* node) {
    if (granny(node) == NULL) __builtin_unreachable();

    if (granny(node)->left == node->parent && node->parent->right == node) {
        leftRotate(tree, node->parent);
        node = node->left;
    } else if (granny(node)->right == node->parent && node->parent->left == node) {
        rightRotate(tree, node->parent);
        node = node->right;
    }
    insert_case5(tree, node);
}

__attribute__((always_inline))
inline void insert_case5(RBTree* tree, const Node* node) {
    if (granny(node) == NULL) __builtin_unreachable();

    setColor(node->parent, black);
    setColor(granny(node), red);
    if (granny(node)->left == node->parent) {
        rightRotate(tree, granny(node));
    } else {
        leftRotate(tree, granny(node));
    }
    insert_case1(tree, node->parent);
}

__attribute__((always_inline))
inline void insert_fixup(RBTree* tree, Node* node) {
    insert_case1(tree, node);
}

Node* insert(RBTree* tree, NodeType val);

Node* find(const RBTree* tree, NodeType key);

__attribute__((always_inline))
inline void move(const Node* const from, Node* const to) {
    to->value = from->value;
}

__attribute__((always_inline))
inline Node* deleter(Node* const me) {
    if (me->left != NULL) {
        Node* del = me->left;
        while (del->right != NULL) del = del->right;
        return del;
    }
    if (me->right != NULL) {
        Node* del = me->right;
        while (del->left != NULL) del = del->left;
        return del;
    }

    return me;
}

Node* child(const Node* node);

Node* sibling(const Node* node);

__attribute__((always_inline))
inline void delete_case2(RBTree*, Node*);

__attribute__((always_inline))
inline void delete_case3(RBTree*, Node*);

__attribute__((always_inline))
inline void delete_case4(RBTree*, Node*);

void delete_case5(RBTree*, Node*);

__attribute__((always_inline))
inline void delete_case7(RBTree*, Node*);

__attribute__((always_inline))
inline void delete_case8(RBTree*, Node*);


__attribute__((always_inline))
inline void delete_case1(RBTree* tree, Node* node) {
    if (node == NULL) __builtin_unreachable();

    if (getColor(node) == red) {
        if (node->parent->left == node) {
            node->parent->left = child(node);
        } else {
            node->parent->right = child(node);
        }
        if (child(node) != NULL) {
            child(node)->parent = node->parent;
        }
        free(node);
        return;
    }

    delete_case2(tree, node);
}

__attribute__((always_inline))
inline void delete_case2(RBTree* tree, Node* node) {
    if (getColor(child(node)) == red) {

        if (child(node) == NULL) __builtin_unreachable();

        setColor(child(node), black);
        if (node->parent == NULL) {
            child(node)->parent = NULL;
            tree->root = child(node);
            free(node);
            return;
        }
        if (node->parent->left == node) {
            node->parent->left = child(node);
        } else {
            node->parent->right = child(node);
        }

        child(node)->parent = node->parent;
        return;
    }

    delete_case3(tree, node);
}

__attribute__((always_inline))
inline void delete_case3(RBTree* tree, Node* node) {
    if (node->parent == NULL) {
        free(node);
        tree->root = NULL;
        return;
    }
    delete_case4(tree, node);
}

__attribute__((always_inline))
inline void delete_case4(RBTree* tree, Node* node) {

    if (getColor(sibling(node)) == red) {

        if (node->parent == NULL) __builtin_unreachable();
        setColor(node->parent, red);
        setColor(sibling(node), black);
        if (node->parent->left == node) {
            leftRotate(tree, node->parent);
            node->parent->left = NULL;
        } else {
            rightRotate(tree, node->parent);
            node->parent->right = NULL;
        }
    }
    delete_case5(tree, node);
}

__attribute__((always_inline))
inline void delete_case6(RBTree* tree, Node* node) {
    if (getColor(node->parent) == red && getColor(sibling(node)) == black &&
    getColor(sibling(node)->left) == black && getColor(sibling(node)->right) == black) {
        setColor(node->parent, black);
        setColor(sibling(node), red);

        if (node->parent == NULL) __builtin_unreachable();
        if (node->parent->left == node) node->parent->left = NULL;
        if (node->parent->right == node) node->parent->right = NULL;

        free(node);
        return;
    }

    delete_case7(tree, node);
}

__attribute__((always_inline))
inline void delete_case7(RBTree* tree, Node* node) {
    if (getColor(sibling(node)) == black && node == node->parent->left &&
    getColor(sibling(node)->left) == red) {
        setColor(sibling(node)->left, black);
        setColor(sibling(node), red);
        rightRotate(tree, sibling(node));
    } else if (getColor(sibling(node)) == black && node == node->parent->right &&
    getColor(sibling(node)->right) == red) {
        setColor(sibling(node)->right, black);
        setColor(sibling(node), red);
        leftRotate(tree, sibling(node));
    }
    delete_case8(tree, node);
}

__attribute__((always_inline))
inline void swapColors(Node* a, Node* b) {
    const Color c = getColor(a);
    setColor(a, b->color);
    setColor(b, c);
}

__attribute__((always_inline))
inline void delete_case8(RBTree* tree, Node* node) {
    if (getColor(sibling(node)) == black && node == node->parent->left &&
    getColor(sibling(node)->right) == red) {
        swapColors(sibling(node), node->parent);
        setColor(sibling(node)->right, black);
        leftRotate(tree, node->parent);
    } else {
        swapColors(sibling(node), node->parent);
        setColor(sibling(node)->left, black);
        rightRotate(tree, node->parent);
    }
    if (node->parent->left == node) {
        node->parent->left = NULL;
    } else {
        node->parent->right = NULL;
    }
    free(node);
}

__attribute__((always_inline))
inline void delete_fixup(RBTree* tree, Node* node) {
    delete_case1(tree, node);
}

void erase(RBTree* tree, NodeType key);

#endif //RBTREE_RBTREE_H
