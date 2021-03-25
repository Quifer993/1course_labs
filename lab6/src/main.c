#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
//#pragma warning(disable : 4996)


typedef struct Tree {
	struct Tree* left;
	struct Tree* right;
	int value;
	int high;
}Tree;


Tree* small_rotate_left(Tree** root) {
	Tree* old_root = *root;
	Tree* old = (*root)->right;

	root = &old;
	old_root->right = (*root)->left;


	(*root)->left = old_root;

	return *root;
}


Tree* small_rotate_right(Tree** root) {
	Tree* old_root = *root;
	Tree* old = (*root)->left;

	root = &old;
	old_root->left = (*root)->right;

	
	(*root)->right = old_root;

	return *root;
}


//ready
int maximum(int a, int b) {
	if (a > b) {
		return a;
	}
	else {
		return b;
	}
}

//ready
int check_high(Tree* tree) {
	if (tree != NULL) {
		return maximum(check_high(tree->left), check_high(tree->right)) + 1;
	}
	return 0;
}


int check_balance(Tree* node) {
	int balance;
	if (node->left == NULL) {
		return balance = node->right->high;
	}
	else if (node->right == NULL) {
		return balance = -(node->left->high);
	}
	else {
		return balance = node->right->high - node->left->high;
	}

	//return node->right->high - node->left->high;
}


void use_rotate_left(Tree* node) {
	int balance = check_balance(node->left);

	if (balance == 2) {
		if (check_balance(node->left->right) == -1) {
			node->left = small_rotate_right(node);
		}
		node->left = small_rotate_left(&(node->left));
		//small_rotate_left(root);
	}
	if (balance == -2) {
		if (check_balance(node->left->left) == 1) {
			node->left = small_rotate_left(&(node->left));
		}
		node->left = small_rotate_right(node->left);
	}
}


void use_rotate_right(Tree* node) {
	int balance = check_balance(node->right);

	if (balance == 2) {
		if (check_balance(node->right->left) == 1) {
			node->right = small_rotate_right(&(node->right));
		}
		node->right = small_rotate_left(&(node->right));
		//small_rotate_left(root);
	}
	if (balance == -2) {
		if (check_balance(node->right->left) == 1) {
			node->right = small_rotate_left(&(node->right));
		}
		node->right = small_rotate_right(&(node->right));
	}

}


void choose_max_high(Tree* node) {
	if (node->left == NULL) {
		node->high = node->right->high + 1;
	}
	else if(node->right == NULL){
		node->high = node->left->high + 1;
	}
	else {
		node->high = max(node->left->high, node->right->high) + 1;
	}
}


void put_node(Tree* root, Tree* node) {

	if (root->value >= node->value) {
		if (root->left != NULL) {
			put_node(root->left, node);
			//*balance = check_balance(root->left);
			use_rotate_left(root);

		}
		else { 
			root->left = node;
			root->left->high = 1;
		}
	}
	else {
		if (root->right != NULL) {
			put_node(root->right, node);
			//*balance = check_balance(root);
			use_rotate_right(root);
		}
		else {
			root->right = node;
			root->right->high = 1;
		}
	}

	choose_max_high(root);
}


void create_avl_tree(int size) {

	Tree* array_num = (Tree*)calloc(size, sizeof(Tree));
	int num;

	if (scanf("%i", &num) == EOF) {
		exit(EXIT_FAILURE);
	}

	array_num[0].value = num;
	array_num[0].high = 1;

	Tree* root = (Tree*)calloc(1, sizeof(Tree));
	root->left = &array_num[0];


	int rotate_flag = 0;
	int balance = 0;
	for (int i = 1; i < size; i++) {
		if(scanf("%i", &num) == EOF) {
			free(array_num);
			free(root);
			exit(EXIT_FAILURE);
		}
		array_num[i].value = num;
		put_node(root->left, &array_num[i]);
		use_rotate_left(root);
	}

	printf("%i", check_high(root->left));
	free(array_num);
	free(root);
}

//ready
int main() {
	int size;
	if (scanf("%i", &size) == EOF) {
		return 0;
	}

	if (size == 0) {
		printf("0");
		return 0;
	}

	create_avl_tree(size);

	return 0;
}
