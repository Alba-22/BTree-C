typedef struct node* BTree;

BTree* createBTree();
void freeBTree(BTree *root);
int insertInBTree(BTree *root, int value);
int removeFromBTree(BTree *root, int value);
int searchInBTree(BTree *root, int value);
int isEmptyBTree(BTree *root);
int totalNodesBTree(BTree *root);