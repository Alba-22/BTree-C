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

BTree createNode();

BTree* createBTree() {
    BTree* root = (BTree*) malloc(sizeof(BTree));
    if (root == NULL) {
        printf("Error: No memory");
        exit(1);
    }
    (*root) = createNode();
    return root;
}

BTree createNode() {
    BTree newNode = (BTree) malloc(sizeof(struct node));
    newNode->totalKeys = 0;
    for (int i = 0; i < MAX_KEYS + 1; i++) {
        newNode->children[i] = NULL;
    }
    return newNode;
}

int isEmptyBTree(BTree *root) {
    if ((*root)->totalKeys == 0) {
        return 1;
    }
    return 0;
}

int isLeafBTree(BTree node) {
    if (node->children[0] == NULL)
        return 1;
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
            if (aux->children[i] != NULL) {
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