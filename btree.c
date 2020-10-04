#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "btree.h"

#define MAX_KEYS (3)

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
    int i;
    for (i = 0; i < MAX_KEYS + 1; i++) {
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
    BTree aux = (*root);
    if (aux == NULL || aux->totalKeys == 0) {
        return 0;
    }
    while (treeLevel) {
        // Porcura em cada chave do nodo atual.
        while (i < aux->totalKeys && aux->keys[i] < value) {
            i++;
        }
        // Verifica se a chave é igual ao valor passado.
        if (i < aux->totalKeys && aux->keys[i] == value) {
            return treeLevel;
        }
        // Caso não ache o valor no nodo atual, procura no próximo nodo.
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

int split(BTree *root, BTree *parent){
    // Pega o valor no meio, para tamanhos impares, e o logo anterior ao meio, para tamanhos pares
    int med = (*root)->keys[(int)floor((MAX_KEYS-1)/2)], medNextIdx = (int)floor((MAX_KEYS-1)/2) + 1;
    // Guarda tambem o id seguinte, para comecar o laco de preenchimento do novo nodo (proximo laco for)
    
    int idx;
    for(idx = medNextIdx; idx < MAX_KEYS; idx++){
        (*root)->keys[idx-1] = (*root)->keys[idx];
    }
    (*root)->totalKeys--;


    // Cria o novo nodo
    BTree right = createNode();
    BTree left = *root;

    for(idx = medNextIdx-1; idx < MAX_KEYS-1;  idx++){
        right->keys[idx - medNextIdx +1] = left->keys[idx];
        // À medida que os valores vao sendo copiados, a contagem de valores no
        // nodo novo aumenta, enquanto a no nodo antigo diminui
        right->totalKeys++;
        left->totalKeys--;
    }

    // Movimentacao dos ponteiros do nodo no split
    for(idx = medNextIdx; idx <= MAX_KEYS; idx++){
        right->children[idx - medNextIdx] = left->children[idx];
        left->children[idx] = NULL;
    }

    if(*parent == left){
        // Nao tem pai
        *parent = createNode();
        (*parent)->children[0] = left;
        (*parent)->children[1] = right;
        (*parent)->keys[0] = med;
        (*parent)->totalKeys++;
    } else if((*parent)->totalKeys +1 <= MAX_KEYS) {
        // Insere no pai
        for (idx = 0; med > (*parent)->keys[idx] && idx < (*parent)->totalKeys; idx++);
        
        int idx2;
        for (idx2 = (*parent)->totalKeys; idx2 > idx; idx2--) {
            (*parent)->keys[idx2] = (*parent)->keys[idx2-1];
        }
        (*parent)->keys[idx] = med;
        (*parent)->totalKeys++;

        for (idx2 = (*parent)->totalKeys + 1; idx2 > idx+1; idx2--) {
            (*parent)->children[idx2] = (*parent)->children[idx2-1];
        }
        (*parent)->children[idx+1] = right;
    }
}

int insertInNode(BTree *root, BTree *parent, int value){
    // Caso em que o nodo nao esta alocado
    if (*root == NULL) {
        return 0;
    }
    BTree aux = *root;

    if(aux->totalKeys == MAX_KEYS){
        //split
        split(root, parent);
    }

    // Caso em que o nodo esta alocado, mas nao tem valor nenhum
    if (aux->totalKeys == 0) {
        aux->totalKeys++;
        aux->keys[0] = value;
        return 1;
    }

    int i;
    // Procura o index onde deve acontecer a insercao
    for (i = 0; value > aux->keys[i] && i < aux->totalKeys; i++);
    
    // Tenta descer pelo ponteiro correspondente ao 'child', na posicao que o novo 'value' deveria estar
    if (insertInNode(&(aux->children[i]), root, value) == 0) {
        // Se a chamada recursiva retornou '0', o nodo atual é folha
        
        // Caso seja possivel inserir naquele nodo
        if (aux->totalKeys < MAX_KEYS -1) {
            int j;
            for (j = aux->totalKeys; j > i; j--) {
                aux->keys[j] = aux->keys[j-1];
            }
            aux->keys[i] = value;
            aux->totalKeys++;
            return 1;
        } else {
            // Caso o nodo esteja cheio, faz o split, e eleva o valor na mediana;

            int idx;
            for (idx = 0; value > (*root)->keys[idx] && idx < (*root)->totalKeys; idx++);
                
            int idx2;
            for (idx2 = (*root)->totalKeys; idx2 > idx; idx2--) {
                (*root)->keys[idx2] = (*root)->keys[idx2-1];
            }
            (*root)->keys[idx] = value;
            (*root)->totalKeys++;


            split(root, parent);
        }
    }

    if((*root)->totalKeys == MAX_KEYS){
        split(root, parent);
    }
    return 1;

}

int insertInBTree(BTree *root, int value){
    insertInNode(root, root, value);
}

void freeBTree(BTree *root) {
    BTree aux = *root;
    // Se não for folha, irá continuar descendo pela árvore
    if (isLeafBTree(aux) == 0) {
        int i;
        // Percorre os filhos do nó (totalKeys + 1 filhos) liberando recursivamente
        for (i = 0; i < aux->totalKeys + 1; i++) {
            freeBTree(&(aux->children[i]));
            // Necessário atribuir NULL ao filho para que a verificação de ser folha funcione
            aux->children[i] = NULL;
        }
    }
    // Se for folha, irá liberar o espaço alocado
    else {
        free(aux);
    }
}

int totalNodesBTree(BTree *root) {
    BTree aux = *root;
    if (aux != NULL) {
        int count = 1;
        int i;
        for (i = 0; i < aux->totalKeys + 1; i++) {
            count += totalNodesBTree(&(aux->children[i]));
        }
        return count;
    }
}

void inOrderBTree(BTree *root) {
    BTree aux = *root;
    if (aux != NULL) {
        int i;
        for (i = 0; i < aux->totalKeys; i++) {
            inOrderBTree(&(aux->children[i]));
            printf("| %d | ", aux->keys[i]);
        }
        inOrderBTree(&(aux->children[i]));
    }
}
