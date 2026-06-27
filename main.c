#include <assert.h>
#include <complex.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    int key;
} NodeType;

int less(const NodeType a, const NodeType b) {
    return a.key < b.key;
}

int eq(const NodeType a, const NodeType b) {
    return a.key == b.key;
}

int bigger(const NodeType a, const NodeType b) {
    return a.key > b.key;
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
    if (node->right == NULL) return;

    if (node->parent == NULL) {
        tree->root = node->right;
        node->right->parent = NULL;
    } else {
        if (node->parent->left == node) {
            node->parent->left = node->right;
        } else {
            node->parent->right = node->right;
        }
        node->right->parent = node->parent;
    }
    node->parent = node->right;

    node->right = node->parent->left;
    if (node->right != NULL) {
        node->right->parent = node;
    }

    node->parent->left = node;
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
    if (node->left == NULL) return;

    if (node->parent == NULL) {
        tree->root = node->left;
        node->left->parent = NULL;
    } else {
        if (node->parent->left == node) {
            node->parent->left = node->left;
        } else {
            node->parent->right = node->left;
        }
        node->left->parent = node->parent;
    }
    node->parent = node->left;

    node->left = node->parent->right;
    if (node->left != NULL) {
        node->left->parent = node;
    }
    node->parent->right = node;
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

void insert_case3(RBTree* tree, Node* node) {
    if (getColor(uncle(node)) == red) {
        setColor(node, red);
        setColor(granny(node), red);
        setColor(uncle(node), black);
        setColor(node->parent, black);
        insert_case1(tree, granny(node));
        return;
    }
    insert_case4(tree, node);
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

Node* insert(RBTree* tree, const NodeType val) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->value = val;
    setColor(node, red);
    node->parent = node->left = node->right = NULL;

    if (tree->root == NULL) {
        insert_fixup(tree, node);
        return node;
    }

    Node* parent = tree->root;
    Node* me = NULL;

    do {
        if (eq(parent->value, val)) {
            me = parent;
        } else if (less(parent->value, val)) {
            me = parent->right;
        } else {
            me = parent->left;
        }
    } while (me != NULL && me != parent && (parent = me, 1));

    if (eq(parent->value, val)) {
        free(node);
        return parent;
    }

    if (less(parent->value, val)) {
        parent->right = node;
    } else {
        parent->left = node;
    }
    node->parent = parent;

    insert_fixup(tree, node);

    return node;
}

Node* find(const RBTree* const tree, const NodeType key) {
    Node* me = tree->root;
    while (me != NULL && !eq(me->value, key)) {
        if (less(me->value, key)) me = me->right;
        else if (bigger(me->value, key)) me = me->left;
    }

    return me;
}

void move(const Node* const from, Node* const to) {
    to->value = from->value;
}

Node* deleter(Node* const me) {
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

Node* child(const Node* const node) {
    if (node->left != NULL) return node->left;
    return node->right;
}

Node* sibling(const Node* const node) {
    if (node == NULL) return NULL;
    if (node->parent == NULL) return NULL;
    if (node->parent->left == node) return node->parent->right;
    return node->parent->left;
}

__attribute__((always_inline))
inline void delete_case1(RBTree*, Node*);

__attribute__((always_inline))
inline void delete_case2(RBTree*, Node*);

__attribute__((always_inline))
inline void delete_case3(RBTree*, Node*);

__attribute__((always_inline))
inline void delete_case4(RBTree*, Node*);

void delete_case5(RBTree*, Node*);

__attribute__((always_inline))
inline void delete_case6(RBTree*, Node*);

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


void delete_case5(RBTree* tree, Node* node) {
    if (getColor(node->parent) == black && getColor(sibling(node)) == black &&
    getColor(sibling(node)->left) == black && getColor(sibling(node)->right) == black) {
        setColor(sibling(node), red);

        if (node->parent == NULL) __builtin_unreachable();
        if (node->parent->left == node) node->parent->left = NULL;
        if (node->parent->right == node) node->parent->right = NULL;

        delete_case1(tree, node->parent);

        free(node);
        return;
    }
    delete_case6(tree, node);
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
    const Color c = a->color;
    a->color = b->color;
    b->color = c;
}

__attribute__((always_inline))
void delete_case8(RBTree* tree, Node* node) {
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

void erase(RBTree* tree, const NodeType key) {
    Node* node = find(tree, key);
    if (node == NULL) {
        return;
    }

    Node* del = deleter(node);
    move(del, node);
    node = del;

    delete_fixup(tree, node);
}

void inorder(Node* x) {
    if (x == NULL) return;
    inorder(x->left);
    printf("%d ", x->value.key);
    inorder(x->right);
}

void print(RBTree* t) {
    inorder(t->root);
}

int main() {
    RBTree t;
    t.root = NULL;
    const clock_t begin = clock();

    char c;
    do {
        scanf("%c", &c);
        if (c == 'i') {
            int x;
            scanf("%d", &x);
            insert(&t, (NodeType){x});
        } else if (c == 'd') {
            int x;
            scanf("%d", &x);
            erase(&t, (NodeType){x});
        } else if (c == 'p') {
            print(&t);
            printf("\n");
        }
    } while (c != 's');

    const clock_t end = clock();
    const double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("%lf", time_spent);
}