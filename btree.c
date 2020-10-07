#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "btree.h"

#define MAX_KEYS (5)

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
    int med = (*root)->keys[(int)floor((MAX_KEYS-1)/2.0)], medNextIdx = (int)floor((MAX_KEYS-1)/2.0) + 1;
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
    return insertInNode(root, root, value);
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


void dealUnderflow(BTree *root, BTree *parent){

    // Root vazia
    if(*root == *parent && (*root)->totalKeys == 0){
        int a = 1; // 
        BTree *aux = root;
        *root = (*root)->children[0];
        free(*aux);
    }
    // Underflow em nodo
    else if((*root)->totalKeys < floor(MAX_KEYS/2.0) && *root != *parent){
        // Underflow

        // Pega o index do nodo atual no nodo pai
        int i; //index do nodo atual no nodo pai
        for(i = 0; i < MAX_KEYS+1; i++){
            if((*parent)->children[i] == *root){
                break;
            };
        }

        // Pega emprestado do irmao da esquerda, se der
            // Maior valor do irmao sobre para separador no pai, 
            // separador do pai desce para o nodo
            // [!] Atencao aos ponteiros quando nao for folha!
        if(i > 0 && (*parent)->children[i-1]->totalKeys > floor(MAX_KEYS/2.0)){
            BTree leftBrother = (*parent)->children[i-1];
            int j;

            // Move os valores chave, para insercao no inicio
            for(j = (*root)->totalKeys; j > 0; j--){
                (*root)->keys[j] = (*root)->keys[j-1];
            }
            
            // valor separador entra no nodo
            (*root)->keys[0] = (*parent)->keys[i-1];
            (*root)->totalKeys++;
            
            // maior valor do irmao vai para a posicao de separador
            (*parent)->keys[i-1] = leftBrother->keys[leftBrother->totalKeys-1];

            // Move os ponteiros, para insercao no inicio
            for(j = (*root)->totalKeys + 1; j > 0; j--){
                (*root)->children[j] = (*root)->children[j-1];
            }
            // Insere o ponteiro, movendo o filho do irmao para o nodo
            (*root)->children[0] = leftBrother->children[leftBrother->totalKeys];

            // Desaponta o ponteiro, retirando o filho do irmao, que agora ja esta no nodo
            leftBrother->children[leftBrother->totalKeys] = NULL;
            leftBrother->totalKeys--;
        }

        // Se nao der na esquerda, tenta no irmao da direita
            // Menor valor do irmao sobre para separador no pai,
            // separador do pai desce para o nodo
            // [!] Atencao aos ponteiros quando nao for folha!
        else if(i < (*parent)->totalKeys && (*parent)->children[i+1]->totalKeys > floor(MAX_KEYS/2.0)){
            BTree rightBrother = (*parent)->children[i+1];

            // Valor separador entra no final do nodo
            (*root)->keys[(*root)->totalKeys] = (*parent)->keys[i];
            (*root)->totalKeys++;

            // Menor valor do irmao a direita vai para a posicao de separador
            (*parent)->keys[i] = rightBrother->keys[0];

            //Insere o primeiro filho do irmao no final do nodo
            (*root)->children[(*root)->totalKeys] = rightBrother->children[0];

            // Move os ponteiros do irmao, retirando o filho que agora ja esta no nodo
            int j;
            for(j = 0; j < rightBrother->totalKeys; j++){
                rightBrother->children[j] = rightBrother->children[j+1];
            }
            rightBrother->children[j] = NULL;
            rightBrother->totalKeys--;

            // Move os valores do irmao
            for(j = 0; j < rightBrother->totalKeys; j++){
                rightBrother->keys[j] = rightBrother->keys[j+1];
            }
        }


        // Nenhum dos irmaos pode emprestar (merge)
            // Juntando com o irmao da esquerda, em ordem,
            // (irmao da esquerda) + (separador) + (nodo em underflow)
            // movimentando os elementos do pai para a retirada do separador
            // [!] Atencao aos ponteiros quando nao for folha!

            // Juntando com o irmao da direita, em ordem,
            // (nodo em underflow) + (separador) + (irmao da direita)
            // movimentando os elementos do pai para a retirada do separador
            // [!] Atencao aos ponteiros quando nao for folha!

            // Se o pai for raiz, e agora nao tiver mais elementos
                // Liberar a raiz e fazer o ponteiro apontar para o nodo unido nessa operacao
        else{
            // merge

            // Tenta merge com o irmao da esquerda
            if(i > 0 && (*parent)->children[i-1] != NULL){
                BTree leftBrother = (*parent)->children[i-1];

                leftBrother->keys[leftBrother->totalKeys] = (*parent)->keys[i-1];
                leftBrother->totalKeys++;

                // Move os valores do nodo para o irmao
                int j;
                for(j = 0; j < (*root)->totalKeys; j++){
                    leftBrother->keys[leftBrother->totalKeys + j] = (*root)->keys[j];
                }

                // Move os ponteiros do dono para o irmao
                for(j = 0; j < (*root)->totalKeys + 1; j++){
                    leftBrother->children[leftBrother->totalKeys + j] = (*root)->children[j];
                }
                leftBrother->totalKeys += (*root)->totalKeys;

                // Move todos os ponteiros e valores do pai uma posicao p a esquerda, a partir do removido
                BTree aux = *root;
                for(j = i; j <= (*parent)->totalKeys; j++){
                    (*parent)->children[j] = (*parent)->children[j+1];
                }
                (*parent)->totalKeys--;
                for(j = i-1; j < (*parent)->totalKeys; j++){
                    (*parent)->keys[j] = (*parent)->keys[j+1];
                }

                free(aux);

            }
            // Tenta merge com o irmao da direita
            else if(i < MAX_KEYS && (*parent)->children[i+1] != NULL){
                BTree rightBrother = (*parent)->children[i+1];

                (*root)->keys[(*root)->totalKeys] = (*parent)->keys[i];
                (*root)->totalKeys++;

                // Move os valores do nodo para o irmao
                int j;
                for(j = 0; j < rightBrother->totalKeys; j++){
                    (*root)->keys[(*root)->totalKeys + j] = rightBrother->keys[j];
                }

                // Move os ponteiros do irmao para o nodo
                for(j = 0; j < rightBrother->totalKeys + 1; j++){
                    (*root)->children[(*root)->totalKeys + j] = rightBrother->children[j];
                }
                (*root)->totalKeys += rightBrother->totalKeys;

                // Move todos os ponteiros e valores do pai uma posicao p a esquerda, a partir do removido
                
                for(j = i+1; j <= (*parent)->totalKeys; j++){
                    (*parent)->children[j] = (*parent)->children[j+1];
                }
                (*parent)->totalKeys--;
                for(j = 0; j < (*parent)->totalKeys; j++){
                    (*parent)->keys[j] = (*parent)->keys[j+1];
                }

                free(rightBrother);
            }



        }
    }
}

int removeFromNode(BTree *root, BTree *parent, int value);

int getAndRemoveMax(BTree *root, BTree *parent, int *max){
    // Para que funcione trocando os valores no caso da inserção no meio da árvore b,
    // basta passar o endereço do separador original como 'max'
    // Dessa forma, ao chegar no final da recursão, o valor a ser removido no meio da árvore já será trocado pelo seu
    // predecessor em ordem, nao comprometendo futuros rebalanceamentos que possam ocorrer na volta das chamadas recursivas
    int returnedValue;
    if(isLeafBTree(*root)){
        *max = (*root)->keys[(*root)->totalKeys-1]; 
        returnedValue = removeFromNode(root, parent, *max);
    }else{
        returnedValue = getAndRemoveMax(&((*root)->children[(*root)->totalKeys]), root, max);
    }
    // Tratar underflow no nodo atual
    dealUnderflow(root, parent);
    return returnedValue; //?
}

int removeFromNode(BTree *root, BTree *parent, int value) {
    if(isLeafBTree(*root)){
        // O nodo atual e folha
        int i;
        //Busca o valor
        for(i = 0; value > (*root)->keys[i] && i < (*root)->totalKeys; i++);
        if (i >= (*root)->totalKeys || value < (*root)->keys[i]){
            // Not found
            return 0;
        }
        int j;
        for(j = i; j < (*root)->totalKeys; j++){
            (*root)->keys[j] = (*root)->keys[j+1];
        }
        (*root)->totalKeys--;

        // Removido com sucesso;
    }else{
        // Nao e folha
        // Verifica se o valor esta no nodo
        int i;
        for(i = 0; value > (*root)->keys[i] && i < (*root)->totalKeys; i++);

        if(i == (*root)->totalKeys || value < (*root)->keys[i]){
            // O valor nao esta no nodo, tenta excluir no filho em que poderia estar
            int removed = removeFromNode(&((*root)->children[i]), root, value);
            if(!removed){
                // Nao encontrou o valor na arvore
                // Not found;
                return 0;
            }
        }else{
            // Encontrou o valor a ser excluido, no index 'i'

            // Busca o maior elemento da sub-arvore a esquerda,
            // coloca no lugar de keys[i], e deleta do nó folha
            int returnedValue = getAndRemoveMax(&((*root)->children[i]), root, &( (*root)->keys[i] ) );

            // Trocou de lugar e removeu, com sucesso
            // Ja lidou com underflow dos filhos dentro da funcao 'getAndRemoveMax'
        }
    }

    // Trata underflow no nodo atual
    dealUnderflow(root, parent);

    return 1; //teste9
}

int removeFromBTree(BTree *root, int value) {
    return removeFromNode(root, root, value);
}
