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
            option = mainMenu();
            // Create BTree
            if (option != 1 && option != MAX_OPTIONS && init == 0) {
                printf("The BTree wasn't created");
                endOption();
            }
            else if (option == 1) {
                if (init == 0) {
                    bt = createBTree();
                    init = 1;
                    printf("BTree created");
                    endOption();
                }
                else {
                    int reset;
                    do {
                        printf("There's already a BTree created:\n");
                        printf("[0] - Keep BTree\n");
                        printf("[1] - Reset BTree\n");
                        printf("Choice: ");
                        scanf("%d", &reset);
                        if (reset == 1) {
                            bt = createBTree();
                            printf("BTree re-created");
                        }
                        else if (reset == 0) {
                            printf("Kept BTree");
                        }
                    } while (reset < 0 || reset > 1);
                    endOption();
                }
            }
            // Insert integer in BTree
            else if (option == 2) {
                endOption();
            }
            // Remove integer from BTree
            else if (option == 3) {
                endOption();
            }
            // Search for a integer in BTree
            else if (option == 4) {
                endOption();
            }
            // Verify if BTree is empty
            else if (option == 5) {
                int isEmpty = isEmptyBTree(bt);
                if (isEmpty == 1) {
                    printf("BTree is empty");
                }
                else {
                    printf("BTree isn't empty");
                }
                endOption();
            }
            // Total number of Nodes
            else if (option == 6) {
                endOption();
            }
            // Destroy BTree
            else if (option == 7) {
                endOption();
            }
            // Print BTree in order
            else if (option == 8) {
                endOption();
            }
            else if (option == MAX_OPTIONS) {
                printf("Leaving...");
                keep = 0;
            }
        } while (option < 1 || option > MAX_OPTIONS);
    }
}

int mainMenu() {
    int option;
    do {
        printf("====================================\n");
        printf("|            BTREE MENU            |\n");
        printf("====================================\n");
        printf("[01] Create BTree                  |\n");
        printf("[02] Insert integer in BTree       |\n");
        printf("[03] Remove integer from BTree     |\n");
        printf("[04] Search for a integer in BTree |\n");
        printf("[05] Verify if BTree is empty      |\n");
        printf("[06] Total number of Nodes         |\n");
        printf("[07] Destroy BTree                 |\n");
        printf("[08] Print BTree in order          |\n");
        printf("[09] Leave Program                 |\n");
        printf("------------------------------------\n");
        printf("Choice: ");
        scanf("%d", &option);
        printf("------------------------------------\n");
    } while (option < 1 || option > MAX_OPTIONS);
    return option;
}

void endOption() {
    printf("\n");
    system("pause");
    // system("cls");
    printf("\e[1;1H\e[2J");
}