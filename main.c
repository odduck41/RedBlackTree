#include <stdio.h>
#include <string.h>
#include <time.h>

#include "RBTree.h"

void inorder(const Node* const x, char* s) {
    if (x == NULL) return;
    inorder(x->left, s);
    sprintf(s + strlen(s), "\t%d[label=\"%d\\n%s\" shape=circle color=%s]\n\0", x, x->value.key, x->value.value,
        x->color==red?"red":"black");
    if (x->left)
        sprintf(s + strlen(s), "\t%d -> %d\n\0", x, x->left);
    if (x->right)
        sprintf(s + strlen(s), "\t%d -> %d\n\0", x, x->right);
    inorder(x->right, s);
}

void print(const RBTree* const t) {
    // inorder(t->root);
    // printf("%d", t->root);
    char s[10000];
    sprintf(s, "digraph G {\n\0");
    inorder(t->root, s);
    sprintf(s + strlen(s), "}\n\0");
    FILE * f = fopen("../tree.dot", "w");
    if (f == NULL) return;
    fputs(s, f);
    fclose(f);
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
            char s[100];
            scanf(" %s", s);
            insert(&t, (NodeType){x, s});
            print(&t);
        } else if (c == 'd') {
            int x;
            scanf("%d", &x);
            erase(&t, (NodeType){x});
            print(&t);
        } else if (c == 'f') {
            int x;
            scanf("%d", &x);
            const Node* e = find(&t, (NodeType){x});
            if (e != NULL) {
                printf("%s", e->value.value);
            }
            printf("\n");
        } else if (c == 'p') {
            print(&t);
            // printf("\n");
        }
    } while (c != 's');

    const clock_t end = clock();
    const double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("%lf", time_spent);
}