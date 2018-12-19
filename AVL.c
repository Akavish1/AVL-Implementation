#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>

struct node {
	int key;
	struct node *left, *right;
	int height;
};

//creates a new node and returns it
struct node *newnode(int newkey) {
	struct node *nnode = (struct node *)malloc(sizeof(struct node));
	nnode->key = newkey;
	nnode->left = NULL;
	nnode->right = NULL;
	nnode->height = 1;
	return nnode;
}

//calculate the height of the tree
int height(struct node *node) {
	if (node == NULL)
		return 0;
	else {
		int leftheight = height(node->left);
		int rightheight = height(node->right);
		if (leftheight > rightheight)
			return(leftheight + 1);
		else return(rightheight + 1);
	}
}

//get node's balance
int getbalance(struct node *t) {
	if (t == NULL)
		return 0;
	return height(t->left) - height(t->right);
}

//prints the maximal value in the tree
struct node* findmaximum(struct node* root) {
	if (root->right == NULL) {
		printf("The biggest value is %d\n", root->key);
		return root;
	}
	findmaximum(root->right);
}

//prints the minimal value in the tree
struct node* findminimum(struct node* root) {
	if (root->left == NULL) {
		printf("The smallest value is %d\n", root->key);
		return root;
	}
	findminimum(root->left);
}

//find the minimal node
struct node *minnode(struct node* node) {
	struct node* i = node;
	while (i->left != NULL)
		i = i->left;
	return i;
}

struct node *rotateright(struct node *t) {
	struct node *tleft = t->left;
	struct node *tright = tleft->right;
	tleft->right = t;
	t->left = tright;
	t->height = max(height(t->left), height(t->right)) + 1;
	tleft->height = max(height(tleft->left), height(tleft->right)) + 1;
	return tleft;
}

struct node *rotateleft(struct node *t) {
	struct node *tright = t->right;
	struct node *tleft = tright->left;
	tright->left = t;
	t->right = tleft;
	t->height = max(height(t->left), height(t->right)) + 1;
	tright->height = max(height(tright->left), height(tright->right)) + 1;
	return tright;
}

// inserts a node and balanced the tree if needed
struct node* insert(struct node* node, int key){
	if (node == NULL)
		return(newnode(key));
	if (key < node->key)
		node->left = insert(node->left, key);
	else if (key > node->key)
		node->right = insert(node->right, key);
	node->height = 1 + max(height(node->left), height(node->right));
	int balance = getbalance(node);
	//L-L
	if (balance > 1 && key < node->left->key)
		return rotateright(node);
	//R-R
	if (balance < -1 && key > node->right->key)
		return rotateleft(node);
	//L-R
	if (balance > 1 && key > node->left->key){
		node->left = rotateleft(node->left);
		return rotateright(node);
	}
	//R-L
	if (balance < -1 && key < node->right->key){
		node->right = rotateright(node->right);
		return rotateleft(node);
	}
	return node;
}

struct node* deletenode(struct node* root, int key) {
		if (root == NULL)
			return root;
		//traversing left
		if (key < root->key)
			root->left = deletenode(root->left, key);
		//traversing right
		else if (key > root->key)
			root->right = deletenode(root->right, key);
		else {
			// one or no child
			if ((root->left == NULL) || (root->right == NULL)) {
				struct node *temp = root->left ? root->left : root->right;
				// no child
				if (temp == NULL) {
					temp = root;
					root = NULL;
				}
				//one child
				else
					*root = *temp;
				free(temp);
			}
			else{
				//two children
				struct node* temp = minnode(root->right);
				root->key = temp->key;
				root->right = deletenode(root->right, temp->key);
			}
		}
		if (root == NULL)
			return root;
		//change height
		root->height = 1 + max(height(root->left), height(root->right));
		//check if tree became unbalanced
		int balance = getbalance(root);
		// L-L
		if (balance > 1 && getbalance(root->left) >= 0)
			return rotateright(root);
		//L-R
		if (balance > 1 && getbalance(root->left) < 0){
			root->left = rotateleft(root->left);
			return rotateright(root);
		}
		//R-R
		if (balance < -1 && getbalance(root->right) <= 0)
			return rotateleft(root);
		//R-L
		if (balance < -1 && getbalance(root->right) > 0){
			root->right = rotateright(root->right);
			return rotateleft(root);
		}
		return root;
}

//prints keys at a given level
void printlevel(struct node* root, int level) {
	if (root == NULL)
		return;
	if (level == 1)
		printf("%d ", root->key);
	else if (level > 1) {
		printlevel(root->left, level - 1);
		printlevel(root->right, level - 1);
	}
}

//levelorder traversal
void levelorder(struct node* node) {
	int h = height(node);
	for (int i = 1; i <= h; i++)
		printlevel(node, i);
}


//preorder traversal
void preorder(struct node* node) {
	if (node == NULL)
		return;
	printf("%d ", node->key);
	preorder(node->left);
	preorder(node->right);
}

// inorder traversal 
void inorder(struct node *root) {
	if (root != NULL) {
		inorder(root->left);
		printf("%d ", root->key);
		inorder(root->right);
	}
}

//postorder traversal
void postorder(struct node* node) {
	if (node == NULL)
		return;
	postorder(node->left);
	postorder(node->right);
	printf("%d ", node->key);
}

//search for a key in the tree
void searchkey(struct node* node, int key) {
	if (node == NULL)
		return;
	if (node->key == key) {
		printf("Key %d is in the tree\n", key);
		return;
	}
	if (node->key < key) {
		if (node->right == NULL) {
			printf("Key %d is not in the tree\n", key);
		}
		searchkey(node->right, key);
	}
	if (node->key > key) {
		if (node->left == NULL) {
			printf("Key %d is not in the tree\n", key);
		}
		searchkey(node->left, key);
	}
}

//finds the successor of a given node
void findsuccessor(struct node* node, struct node* suc, int key) {
	if (node == NULL)
		return;
	if (node->key == key) {
		if (node->right != NULL) {
			struct node* tmp = node->right;
			while (tmp->left)
				tmp = tmp->left;
			suc = tmp;
		}
		printf("Successor is %d\n", suc->key);
		return;
	}
	if (node->key > key) {
		suc = node;
		findsuccessor(node->left, suc, key);
	}
	printf("Successor is %d\n", suc->key);
}

//converts the bst to two doubly linked lists (one for each subtree) and merge them
struct node* bsttoll(struct node* root) {
	if (root == NULL)
		return root;
	if (root->left != NULL) {
		struct node* left = bsttoll(root->left);
		for (; left->right != NULL; left = left->right);
		left->right = root;
		root->left = left;
	}
	if (root->right != NULL) {
		struct node* right = bsttoll(root->right);
		for (; right->left != NULL; right = right->left);
		right->left = root;
		root->right = right;
	}
	return root;
}

//define the dll's head
struct node* createdll(struct node *root) {
	if (root == NULL)
		return root;
	root = bsttoll(root);
	while (root->left != NULL)
		root = root->left;
	return (root);
}

void balancefactors(struct node *node) {
		if (node == NULL)
			return;
		printf("key = %d, balance factor = %d\n", node->key, getbalance(node));
		balancefactors(node->left);
		balancefactors(node->right);
}

int main() {
	struct node* tree = newnode(11);
	int input = 0;
	int newkey = 0;
	int keytosearch = 0;
	while (1) {
		printf("1.Load AVL\n2.Insert new node\n3.Delete node\n4.Print Levelorder\n5.Print Preorder\n6.Print Inorder\n7.Print Postorder\n8.AVL - SearchKey\n9.PrintTree- Minimum\n10.PrintTree- Maximum\n11.PrintTree- Successor by Key\n12.Convert to preorder doubly link list\n13.Print balance factors\n14.Exit\n");
		scanf("%d", &input);
		switch (input) {
		case 1:
			printf("\n\n");
			insert(tree, 14);
			insert(tree, 9);
			insert(tree, 17);
			insert(tree, 7);
			insert(tree, 11);
			insert(tree, 15);
			insert(tree, 18);
			insert(tree, 8);
			insert(tree, 10);
			insert(tree, 13);
			insert(tree, 16);
			insert(tree, 12);
			printf("AVL loaded\n\n");
			break;
		case 2:
			printf("\nEnter key to add\n");
			scanf("%d", &newkey);
			printf("\n\n");
			insert(tree, newkey);
			break;
		case 3:
			printf("\nEnter key to delete\n");
			scanf("%d", &newkey);
			printf("\n\n");
			deletenode(tree, newkey);
			printf("\n\n");
			break;
		case 4:
			printf("\nLevelorder - \n\n");
			levelorder(tree);
			printf("\n\n");
			break;
		case 5:
			printf("\nPreorder - \n\n");
			preorder(tree);
			printf("\n\n");
			break;
		case 6:
			printf("\nInorder - \n\n");
			inorder(tree);
			printf("\n\n");
			break;
		case 7:
			printf("\nPostorder - \n\n");
			postorder(tree);
			printf("\n\n");
			break;
		case 8:
			printf("\nEnter key to search\n");
			scanf("%d", &keytosearch);
			printf("\n\n");
			searchkey(tree, keytosearch);
			printf("\n\n");
			break;
		case 9:
			printf("\n\n");
			findminimum(tree);
			printf("\n\n");
			break;
		case 10:
			printf("\n\n");
			findmaximum(tree);
			printf("\n\n");
			break;
		case 11:
			printf("\nEnter key to find it's successor\n");
			scanf("%d", &keytosearch);
			printf("\n\n");
			struct node* suc = newnode(keytosearch);
			findsuccessor(tree, suc, keytosearch);
			printf("\n\n");
			break;
		case 12:
			printf("\nBinary search tree to doubly linked list\n\n");
			createdll(tree);
			printf("Conversion successful\n\n");
			break;
		case 13:
			printf("\n\n");
			balancefactors(tree);
			printf("\n\n");
			break;
		case 14:
			exit(1);
		default:
			printf("Invalid input\n");
		}
	}


}