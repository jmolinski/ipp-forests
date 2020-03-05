#include "bst.h"
#include <stdio.h>
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
    Tree tree = malloc(sizeof(struct Node));
    if (tree == NULL) {
        exit(1);
    }

    tree->key = key;
    tree->value = NULL;
    tree->left = left;
    tree->right = right;
    return tree;
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

Tree bstGetOrCreate(Tree t, char *key) {
    Tree node = bstGet(t, key);
    if (node == NULL) {
        node = bstInsert(t, key);
    }

    return node;
}

Tree bstInsert(Tree t, char *key) {
    if (t == NULL) {
        return createNode(key, NULL, NULL);
    }

    int cmpResult = BST_COMPARE_KEYS(t->key, key);
    if (cmpResult == 0) {
        return t;
    } else {
        Tree *branch = cmpResult < 0 ? &(t->left) : &(t->right);
        if (*branch == NULL) {
            *branch = createNode(key, NULL, NULL);
            return *branch;
        }
        return bstInsert(*branch, key);
    }
}

static Tree minValueNode(Tree tree)
{
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
        if (root->left == NULL) {
            Tree temp = root->right;
            freeNode(root);
            return temp;
        } else if (root->right == NULL) {
            Tree temp = root->left;
            freeNode(root);
            return temp;
        }

        Tree temp = minValueNode(root->right);
        free(root->key);  // ??
        freeTree(root->value); // ??
        root->key = temp->key;
        root->value = temp->value;
        // tutaj bedzie memory leak
        root->right = bstDelete(root->right, temp->key);
    }
    if (cmpResult < 0) {
        root->left = bstDelete(root->left, key);
    } else {
        root->right = bstDelete(root->right, key);
    }

    return root;
}

Tree bstSortedDisplay(Tree tree) {
    return NULL;
}
