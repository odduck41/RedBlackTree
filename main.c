#include <stdio.h>
#include <time.h>

#include "RBTree.h"

void inorder(const Node* const x) {
    if (x == NULL) return;
    inorder(x->left);
    printf("%d %s ", x->value.key, x->value.value);
    inorder(x->right);
}

void print(const RBTree* const t) {
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
            char s[100];
            scanf(" %s", s);
            insert(&t, (NodeType){x, s});
        } else if (c == 'd') {
            int x;
            scanf("%d", &x);
            erase(&t, (NodeType){x});
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
            printf("\n");
        }
    } while (c != 's');

    const clock_t end = clock();
    const double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("%lf", time_spent);
}