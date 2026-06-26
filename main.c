#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

#define granny(node) (\
    node==NULL?\
        NULL\
    :\
        (\
        node->parent==NULL?\
            NULL\
        :\
            node->parent->parent\
        )\
)

#define uncle(node) (\
    node==NULL?\
        NULL\
    :\
        (\
            node->parent==NULL?\
                NULL\
            :\
                (\
                    granny(node)==NULL?\
                        NULL\
                    :\
                        (\
                            granny(node)->left == node->parent?\
                                granny(node)->right\
                            :\
                                granny(node)->left\
                        )\
                )\
        )\
)

#define getColor(node) (node==NULL?(black):(node->color))
#define setColor(node, col) ((node==NULL)?(node->color):(node->color=col))


void fixup(RBTree* tree, Node* node);

#define case5(tree,node) (\
    setColor(node->parent, black),\
    setColor(granny(node), red),\
    (granny(node)->left==node->parent?rightRotate(tree, node->parent):leftRotate(tree,node->parent)),\
    fixup(tree, node->parent),\
    0\
)

#define case4(tree,node) (\
    (granny(node)->left == node->parent && node->parent->right == node)?\
        (leftRotate(tree,node),(case5(tree,node->left)),0)\
    :\
        (\
            granny(node)->right == node->parent && node->parent->left == node?\
                (rightRotate(tree, node),(case5(tree,node->left)),0)\
                :\
                (case5(tree,node),0)\
        )\
)

#define case3(tree, node) (\
    getColor(uncle(node)) == red?\
        (\
            setColor(node,red),\
            setColor(granny(node),red),\
            setColor(uncle(node),black),\
            setColor(node->parent,black),\
            fixup(tree,granny(node)),\
            0\
        )\
    :\
        (case4(tree,node),0)\
)

#define case2(tree, node) (\
    getColor(node->parent)==black?\
        (0)\
    :\
        (case3(tree, node),0)\
)

#define case1(tree, node) (\
    (node->parent==NULL)?\
        (node->color=black,0)\
    :\
        (case2(tree,node),0)\
)

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