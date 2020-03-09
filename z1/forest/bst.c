#include "bst.h"
#include "input_handler.h"
#include "safe_malloc.h"
#include <stdlib.h>
#include <string.h>

/* frees the node but does not free its left/right subtrees */
void freeNode(Tree node) {
    if (node != NULL) {
        free(node->key);
        freeTree(node->value);
        free(node);
    }
}

/* frees the node and its subtrees recursively */
void freeTree(Tree tree) {
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

/* creates and returns a new Node struct */
Tree createNode(char *key, Tree left, Tree right) {
    Tree node = safeMalloc(sizeof(struct Node));
    node->key = copyString(key);
    node->value = NULL;
    node->left = left;
    node->right = right;
    return node;
}

/* returns Node with the specified key
 * or NULL if key is not found in the tree */
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

/* returns Node with the specified key
 * if the Node does not exist yet it will be created and added to the tree */
Tree bstGetOrCreate(Tree *t, char *key) {
    Tree node = bstGet(*t, key);
    if (node == NULL) {
        node = bstInsert(t, key);
    }

    return node;
}

/* adds a Node with the specified key to the tree
 * if a Node with the specified key already exists then no action will
 * be performed.
 * returns the Node */
Tree bstInsert(Tree *treePtr, char *key) {
    if (*treePtr == NULL) {
        *treePtr = createNode(key, NULL, NULL);
        return *treePtr;
    }

    Tree tree = *treePtr;
    int cmpResult = BST_COMPARE_KEYS(tree->key, key);
    if (cmpResult == 0) {
        return tree;
    } else {
        Tree *branch = cmpResult < 0 ? &(*treePtr)->left : &(*treePtr)->right;
        if (*branch == NULL) {
            *branch = createNode(key, NULL, NULL);
            return *branch;
        }
        return bstInsert(branch, key);
    }
}

/* returns Node with the minimal key value
   returns NULL if tree is Null */
static inline Tree minValueNode(Tree tree) {
    Tree current = tree;
    while (current && current->left != NULL)
        current = current->left;

    return current;
}

/* deletes Node with the specified key if it can be found in the tree
 * returns new tree root */
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

/* calls printLine on all Node keys in the tree in sorted order
 * printLine is an argument - pointer to a function taking a string as argument
 */
void bstDisplaySorted(Tree tree, void (*printLine)(char *)) {
    if (tree != NULL) {
        bstDisplaySorted(tree->right, printLine);
        printLine(tree->key);
        bstDisplaySorted(tree->left, printLine);
    }
}
