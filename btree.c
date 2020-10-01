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
  BTree* bTree = (BTree*) malloc(sizeof(struct node));
  if (bTree == NULL) {
    printf("No memory");
    exit(1);
  }
  (*bTree)->totalKeys = 1;
  return &bTree;
}