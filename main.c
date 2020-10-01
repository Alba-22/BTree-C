#include <stdio.h>
#include <stdlib.h>

#include "btree.h"

#define MAX_OPTIONS 9

int mainMenu();
void endOption();

void main() {
    BTree* bt;
    int option;
    short int keep = 1, init = 0;
    while (keep) {
        do {
            // Create BTree
            option = mainMenu();
            if (option != 1 && option != MAX_OPTIONS && init == 0) {
                printf("The BTree wasn't created\n");
                endOption();
            }
            if (option == 1) {
                bt = createBTree();
                init = 1;
                printf("BTree created\n");
                endOption();
            }
            // Insert integer in BTree
            if (option == 2) {
                endOption();
            }
            // Remove integer from BTree
            if (option == 3) {
                endOption();
            }
            // Search for a integer in BTree
            if (option == 4) {
                endOption();
            }
            // Verify if BTree is empty
            if (option == 5) {
                endOption();
            }
            // Total number of Nodes
            if (option == 6) {
                endOption();
            }
            // Destroy BTree
            if (option == 7) {
                endOption();
            }
            // Print BTree in order
            if (option == 8) {
                endOption();
            }
            if (option == MAX_OPTIONS) {
                printf("Leaving...");
                keep = 0;
            }
        } while (option < 1 || option > MAX_OPTIONS);
    }
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
        printf("[05] Verify if BTree is empty\n");
        printf("[06] Total number of Nodes\n");
        printf("[07] Destroy BTree\n");
        printf("[08] Print BTree in order\n");
        printf("[09] Leave Program\n");
        printf("------------------------------\n");
        printf("Choice: ");
        scanf("%d", &option);
    } while (option < 1 || option > MAX_OPTIONS);
    return option;
}

void endOption() {
    system("pause");
    system("cls");
}