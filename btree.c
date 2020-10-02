#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "btree.h"

#define MAX_KEYS (4)

struct node {
    int totalKeys;
    int keys[MAX_KEYS];
    struct node *children[MAX_KEYS + 1];
};

BTree* createBTree() {
    BTree* root = (BTree*) malloc(sizeof(BTree));
    if (root == NULL) {
        printf("Error: No memory");
        exit(1);
    }
    (*root) = (BTree) malloc(sizeof(struct node));
    (*root)->totalKeys = 0;
    return root;
}

int isEmptyBTree(BTree *root) {
    if ((*root)->totalKeys == 0) {
        return 1;
    }
    return 0;
}

int searchInBTree(BTree *root, int value) {
    int treeLevel = 1, i = 0;
    BTree aux;
    aux = (*root);
    if (aux == NULL || aux->totalKeys == 0) {
        return 0;
    }
    while (treeLevel) {
        // Search in each key of current node.
        while (i < aux->totalKeys && aux->keys[i] < value) {
            i++;
        }
        // Check if key is equal to value, if not search in next node
        if (i < aux->totalKeys && aux->keys[i] == value) {
            return treeLevel;
        }
        else {
            if (aux->children != NULL) {
                aux = aux->children[i];
                i = 0;
                treeLevel++;
            }
            else {
                return 0;
            }
        }

    }
    return 0;
}