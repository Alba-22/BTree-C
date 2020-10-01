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