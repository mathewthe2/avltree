#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define DEBUG 0
struct treeNode{
	int key;
	char fruit[50];
	struct treeNode *left;
	struct treeNode *right;
};
typedef struct treeNode treeNode;

int maximum(int a, int b) {
	return (a > b ? a : b);
}

int get_height(treeNode *currentNode) {
	if(currentNode == NULL)
		return -1; //empty tree
	else {
		int height = 1 + maximum(get_height(currentNode->left), get_height(currentNode->right));
		return height;
	}
}


int get_balance(treeNode *currentNode) {
	if(currentNode == NULL)
		return 0;
	else
		return get_height(currentNode->left) - get_height(currentNode->right);
}

treeNode* R_rotation(treeNode *parent) {
	#if DEBUG
		printf("R_rotation on %d\n", parent->key);
	#endif
	treeNode *child = parent->left;
	parent->left = child->right;
	child->right = parent;
	return child;
}
treeNode* L_rotation(treeNode *parent) {
	#if DEBUG
		printf("L_rotation on %d\n", parent->key);
	#endif
	treeNode *child = parent->right;
	parent->right = child->left;
	child->left = parent;
	return child;
}
treeNode* LR_rotation(treeNode *parent) {
	#if DEBUG
		printf("LR_rotation on %d\n", parent->key);
	#endif
	treeNode *child = parent->left;
	parent->left =  L_rotation(child);
	return R_rotation(parent);
}
treeNode* RL_rotation(treeNode *parent) {
	#if DEBUG
		printf("RL_rotation on %d\n", parent->key);
	#endif
	treeNode *child = parent->right;
	parent->right =  R_rotation(child);
	return L_rotation(parent);
}
treeNode* balance_tree(treeNode *currentNode) {
	int height_diff = get_balance ( currentNode);
	if( height_diff > 1){
		if(get_balance( currentNode -> left ) > 0){
			//Case 1 Right Rotation
			currentNode = R_rotation(currentNode);
		} else {
			//Case 3 Double Rotation: Left + Right
			currentNode = LR_rotation(currentNode);
		}
	} else if(height_diff < -1 ) {
		if(get_balance( currentNode ->right ) < 0){
			//Case 2 Left Rotation
			currentNode = L_rotation(currentNode);

		} else {
			//Case 4 Double Rotation: Right + Left
			currentNode = RL_rotation(currentNode);
		}
	}

	return currentNode;
}
treeNode *Insert(treeNode *currentNode, int key, char fruit[]){
if(currentNode  == NULL){
        #if DEBUG
             printf("Added key  %d\n",key);
        #endif
        currentNode  = (treeNode*)malloc(sizeof(treeNode));
        currentNode ->key = key;
				strcpy(currentNode ->fruit, fruit);
        currentNode ->left = currentNode ->right = NULL;

    }else if(key > currentNode ->key){

        currentNode ->right = Insert(currentNode ->right, key, fruit);
        currentNode  = balance_tree( currentNode );
    }else if(key < currentNode ->key){

        currentNode ->left = Insert(currentNode ->left, key, fruit);
        currentNode  = balance_tree( currentNode );
    }else{
        printf("fail! - duplicated key\n");
        exit(-1);
    }
    return currentNode ;
}
treeNode *Find(treeNode * currentNode, int key){
	if(currentNode ==NULL){
		//Element is not found
		return NULL;
	}
	//printf("Acces node %d \n", currentNode -> key);
	if(key > currentNode->key) {
		// Search in the right sub tree
		return Find(currentNode -> right, key);
	} else if(key < currentNode->key) {
		// Search in the left sub tree
		return Find(currentNode->left, key);
	}else{
		// Element found
		return currentNode;
	}

}
treeNode* FindMin(treeNode *currentNode){
	if(currentNode==NULL){
		//no element in tree
		return NULL;
	}
	if (currentNode ->left != NULL)
		return FindMin(currentNode->left);
	else
		return currentNode;
}
treeNode* FindMax(treeNode *currentNode){
	if(currentNode==NULL){
		//no element in tree
		return NULL;
	}
	if (currentNode ->right != NULL)
		return FindMax(currentNode->right);
	else
		return currentNode;
}


treeNode* delete(treeNode *currentNode, int key){
	if (currentNode==NULL) {
		return NULL;
	}
	if (key < currentNode->key ) {
		currentNode->left = delete(currentNode->left, key);

	} else if (key > currentNode->key) {
					currentNode->right = delete(currentNode->right, key);

	} else {
		if ((currentNode->left == NULL) || (currentNode->right == NULL)){
			treeNode *temp = currentNode->left ? currentNode->left : currentNode->right;;

			if (temp == NULL)
			{
				temp = currentNode;
				currentNode = NULL;
			}
			else
				*currentNode = *temp;

			free(temp);
		} else {
			treeNode* temp = FindMin(currentNode->right);
			currentNode->key = temp->key;
			currentNode->right = delete(currentNode->right, temp->key);
		}
	}
	if (currentNode == NULL)
		return currentNode;
	currentNode = balance_tree(currentNode);
	return currentNode;
}


void PrintInorder(treeNode *currentNode, FILE *f){
        if(currentNode ==NULL){
                return;
        }
        PrintInorder(currentNode->left, f);
				fprintf(f, "%d"", ""%s\n", currentNode->key, currentNode->fruit);
				PrintInorder(currentNode->right, f);
}
int main(){
	treeNode *root = NULL;
	FILE *f = fopen("output.txt", "w");
	int control = 0;
	while (control >= 0) {
		scanf("%d", &control);
		if (control == 0)
		  	root==NULL? fprintf(f, "-1\n") : PrintInorder(root, f);
		if (control == 1) {
			int key;
			char fruit[50];
			scanf("%d%s", &key, fruit);
			root = Insert(root, key, fruit);
		}
		if (control == 2) {
			int key;
			scanf("%d", &key);
			root = delete(root, key);
		}
		if (control == 3) {
			int key;
			scanf("%d", &key);
			treeNode * currentNode;
			currentNode = Find(root, key);
			fprintf(f, "%d"", ""%s\n", currentNode->key, currentNode->fruit);
		}
	}
	fclose(f);




	/*
	root = Insert(root, 10, "Apple");
	root = Insert(root, 50, "Orange");
	root = delete(root, 10);
	root = Insert(root, 40, "Mango");
	root = Insert(root, 30, "Banana");
	*/

	return 0;
}
