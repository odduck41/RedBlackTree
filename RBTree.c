#include "RBTree.h"

#include <string.h>

int less(const NodeType a, const NodeType b) {
    return a.key < b.key;
}

int eq(const NodeType a, const NodeType b) {
    return a.key == b.key;
}

int bigger(const NodeType a, const NodeType b) {
    return a.key > b.key;
}

void* allocator(const int key, const char* value) {
    Node* res = malloc(sizeof(Node));
    res->parent = res->left = res->right = NULL;

    char* cp = malloc(strlen(value) * sizeof(char));

    if (strcpy(cp, value) == cp) {
        res->value = (NodeType){key, cp};
    } else {
        free(cp);
    }

    return res;
}

void deallocator(void* node) {
    free(((Node*)node)->value.value);
    free(node);
}

void leftRotate(RBTree * const tree, Node * const node) {
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

void rightRotate(RBTree * const tree, Node * const node) {
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

void insert_case3(RBTree * tree, Node * node) {
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

Node * insert(RBTree *tree, const NodeType val) {
    Node* node = (Node*)allocator(val.key, val.value);
    setColor(node, red);

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
        deallocator(node);
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

Node * find(const RBTree * const tree, const NodeType key) {
    Node* me = tree->root;
    while (me != NULL && !eq(me->value, key)) {
        if (less(me->value, key)) me = me->right;
        else if (bigger(me->value, key)) me = me->left;
    }

    return me;
}

Node * child(const Node * const node) {
    if (node->left != NULL) return node->left;
    return node->right;
}

Node * sibling(const Node * const node) {
    if (node == NULL) return NULL;
    if (node->parent == NULL) return NULL;
    if (node->parent->left == node) return node->parent->right;
    return node->parent->left;
}

void delete_case5(RBTree * tree, Node * node) {
    if (getColor(node->parent) == black && getColor(sibling(node)) == black &&
        getColor(sibling(node)->left) == black && getColor(sibling(node)->right) == black) {
        setColor(sibling(node), red);

        if (node->parent == NULL) __builtin_unreachable();
        if (node->parent->left == node) node->parent->left = NULL;
        if (node->parent->right == node) node->parent->right = NULL;

        delete_case1(tree, node->parent);

        deallocator(node);
        return;
    }
    delete_case6(tree, node);
}

void erase(RBTree *tree, const NodeType key) {
    Node* node = find(tree, key);
    if (node == NULL) {
        return;
    }

    Node* del = deleter(node);
    move(del, node); // MEMORY LEAK!!!
    node = del;

    delete_fixup(tree, node);
}
