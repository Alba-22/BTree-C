#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "btree.h"

#define MAX_KEYS (1024)

struct node {
  int totalKeys;
  int keys[MAX_KEYS];
  struct node *children[MAX_KEYS + 1];
};