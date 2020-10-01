#include <stdio.h>
#include <stdlib.h>

#include "btree.c"
#include "btree.h"

void main() {

}

int mainMenu() {
  int option;
  do {
      printf("==============================\n");
      printf("             MENU             \n");
      printf("==============================\n");
      printf("[01] Create BTree\n");
      printf("[02] Insert integer in BTree\n");
      printf("[03] Remove integer from BTree\n");
      printf("[04] Search for a integer in BTree\n");
      printf("[05] Remover Pares\n");
      printf("[06] Verify if BTree is empty\n");
      printf("[07] Total number of Nodes\n");
      printf("[08] Destroy BTree\n");
      printf("[09] Leave Program\n");
      printf("------------------------------\n");
      printf("Choice: ");
      scanf("%d", &option);
  } while (option < 1 || option > 9);
  return option;
}