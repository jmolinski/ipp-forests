#include "bst.h"
#include "input_handler.h"
#include "safe_malloc.h"
#include <stdlib.h>
#include <string.h>

void freeNode(Tree node) {
    // this frees the node but does not free its left/right subtrees
    if (node != NULL) {
        free(node->key);
        freeTree(node->value);
        free(node);
    }
}

void freeTree(Tree tree) {
    // frees the node and its subtrees
    if (tree != NULL) {
        if (tree->left != NULL) {
            freeTree(tree->left);
        }
        if (tree->right != NULL) {
            freeTree(tree->right);
        }
        freeNode(tree);
    }
}

Tree createNode(char *key, Tree left, Tree right) {
    Tree node = safeMalloc(sizeof(struct Node));
    node->key = copyString(key);
    node->value = NULL;
    node->left = left;
    node->right = right;
    return node;
}

Tree bstGet(Tree t, char *key) {
    if (t == NULL) {
        return t;
    }

    int cmpResult = BST_COMPARE_KEYS(t->key, key);
    if (cmpResult == 0) {
        return t;
    } else if (cmpResult < 0) {
        return bstGet(t->left, key);
    } else {
        return bstGet(t->right, key);
    }
}

Tree bstGetOrCreate(Tree *t, char *key) {
    Tree node = bstGet(*t, key);
    if (node == NULL) {
        node = bstInsert(t, key);
    }

    return node;
}

Tree bstInsert(Tree *treePtr, char *key) {
    if (*treePtr == NULL) {
        *treePtr = createNode(key, NULL, NULL);
        return *treePtr;
    }

    Tree tree = *treePtr;
    int cmpResult = BST_COMPARE_KEYS(tree->key, key);
    if (cmpResult == 0) {
        return tree;
    } else { // TODO duplicated code
        if (cmpResult < 0) {
            if ((*treePtr)->left == NULL) {
                (*treePtr)->left = createNode(key, NULL, NULL);
                return (*treePtr)->left;
            }
            return bstInsert(&(*treePtr)->left, key);
        } else {
            if ((*treePtr)->right == NULL) {
                (*treePtr)->right = createNode(key, NULL, NULL);
                return (*treePtr)->right;
            }
            return bstInsert(&(*treePtr)->right, key);
        }
    }
}

static inline Tree minValueNode(Tree tree) {
    Tree current = tree;
    while (current && current->left != NULL)
        current = current->left;

    return current;
}

Tree bstDelete(Tree root, char *key) {
    if (root == NULL) {
        return root;
    }
    int cmpResult = BST_COMPARE_KEYS(root->key, key);

    if (cmpResult == 0) {
        if (root->left == NULL || root->right == NULL) {
            Tree temp = root->left == NULL ? root->right : root->left;
            freeNode(root);
            return temp;
        }

        Tree temp = minValueNode(root->right);
        free(root->key);
        freeTree(root->value);
        root->key = copyString(temp->key);
        root->value = temp->value; // move instead of copying the data
        temp->value = NULL; // to prevent freeing this memory on temp's delete
        root->right = bstDelete(root->right, temp->key);
    }
    if (cmpResult < 0) {
        root->left = bstDelete(root->left, key);
    } else {
        root->right = bstDelete(root->right, key);
    }

    return root;
}

void bstDisplaySorted(Tree tree, void (*printLine)(char *)) {
    if (tree != NULL) {
        bstDisplaySorted(tree->right, printLine);
        printLine(tree->key);
        bstDisplaySorted(tree->left, printLine);
    }
}
