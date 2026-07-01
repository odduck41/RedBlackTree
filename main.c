#include <stdio.h>
#include <stdlib.h>

#include "RedBlackTree.h"
#include <string.h>

char s[10000];

void printNode(const Node* const x) {
    sprintf(s + strlen(s), "\t%lld[label=\"%d\\n%s\" shape=circle color=%s]\n", (unsigned long long)x, x->vertex.key, x->vertex.value,
    x->color==red?"red":"black");
    if (x->left)
        sprintf(s + strlen(s), "\t%lld -> %lld\n", (unsigned long long)x, (unsigned long long)x->left);
    if (x->right)
        sprintf(s + strlen(s), "\t%lld -> %lld\n", (unsigned long long)x, (unsigned long long)x->right);
}

void print(const RBTree * const tree) {
    sprintf(s, "digraph G {\n");
    inorder(tree->root, printNode);
    sprintf(s + strlen(s), "}\n");

    FILE * f = fopen("../tree.dot", "w");
    if (f == NULL) return;
    fputs(s, f);
    fflush(f);
    fclose(f);
}

int main() {
    RBTree t = (RBTree){NULL, 1}; // NOLINT

    char c;
    do {
        scanf(" %c", &c);

        char str[100];
        scanf("%s", str);

        const int x = (int)strtol(str, NULL, 10); // NOLINT
        switch (c) {
            case 'i':
                scanf(" %s", s);
                insert(&t, (ValueType){x, s});
                break;
            case 'f':
                const Node * f = find(&t, (ValueType){x, s});
                if (f == NULL) continue;
                printf("%s", f->vertex.value);
                break;
            case 'd':
                delete(&t, (ValueType){x, s});
                break;
            default:

        }
        print(&t);
    } while (c != '0');

    return 0;
}