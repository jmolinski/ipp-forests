#ifndef FOREST_BST_H
#define FOREST_BST_H

#define BST_COMPARE_KEYS(a, b) strcmp(a, b)

struct Node;
typedef struct Node *Tree;

struct Node {
    char *key;
    Tree value;

    Tree left, right;
};

void freeTree(Tree);
Tree bstGet(Tree, char *);
Tree bstGetOrCreate(Tree*, char *);
Tree bstInsert(Tree*, char *);
Tree bstDelete(Tree, char *);
void bstDisplaySorted(Tree, void (*)(char*));

#endif // FOREST_BST_H
