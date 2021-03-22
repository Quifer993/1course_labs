#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
#pragma warning(disable : 4996)


typedef struct Tree {
	struct Tree* left;
	struct Tree* right;
	int value;
	int balance;
}Tree;


void small_rotate_left(Tree* root) {
	Tree* old_root = root;

	if (old_root->left != NULL) {
		old_root->left = root->left->right;
	}
	root = root->right;
	root->left = old_root;


}


Tree* small_rotate_right(Tree* root) {



	Tree* old_root_r = root->right;
	Tree* old_root_1 = root->right;
	Tree* old_root_2 = root->left;
	Tree* old_root_3 = root->left->right;



	if (root->left->right != NULL) {
		old_root->left = root->left->right;
	}
	else {
		old_root->left = NULL;
	}

	root = root->left;
	root->right = old_root;

	//root->right->balance = abs(root->right->left->balance) - abs(root->right->right->balance);
	//root->balance = abs(root->left->balance) - abs(root->right->balance);
	return root;
}


void big_rotate_left(Tree* node) {
	small_rotate_right(node);
	small_rotate_left(node);
}


void big_rotate_right(Tree* node) {
	small_rotate_left(node);
	small_rotate_right(node);

}



int maximum(int a, int b) {
	if (a > b) {
		return a;
	}
	else {
		return b;
	}
}


int check_high(Tree* tree) {
	if (tree != NULL) {
		return maximum(check_high(tree->left), check_high(tree->right)) + 1;
	}
	return 0;
}


void use_rotate(Tree* root, int* rotate_flag) {
	if (*rotate_flag != 0) {

		if (*rotate_flag == 1) {
			if (root->balance == 2) {
				small_rotate_left(root);
			}
			if (root->balance == -2) {
				big_rotate_right(root);
			}
		}
		else {
			if (root->balance == -2) {
				root = small_rotate_right(root);
			}
			if (root->balance == 2) {
				big_rotate_left(root);
			}
		}

		*rotate_flag = root->balance;
	}
}


int check_balance(Tree* node) {
	return check_high(node->right) - check_high(node->left);
}


void put_node(Tree* root, Tree* node, int* rotate_flag) {


	if (root->value >= node->value) {
		if (root->left != NULL) {
			put_node(root->left, node, rotate_flag);
			root->balance = check_balance(root);
			use_rotate(root, rotate_flag);

		}
		else { 
			root->left = node;
			root->balance -= 1;

			if (root->balance == -1) {
				*rotate_flag = -1;
			}
		}
	}
	else {

		if (root->right != NULL) {
			put_node(root->right, node, rotate_flag);
			root->balance = check_balance(root);
			use_rotate(root, rotate_flag);
		

		}
		else {
			root->right = node;
			root->balance += 1;

			if (root->balance == 1) {
				*rotate_flag = 1;
			}
		}
	}

}


void create_avl_tree(Tree* array_num,int size) {
	
	int num;

	if (scanf("%i", &num) == EOF) {
		exit(EXIT_FAILURE);
	}

	array_num[0].value = num;
	Tree root = array_num[0];
	int rotate_flag = 0;
	for (int i = 1; i < size; i++) {
		if(scanf("%i", &num) == EOF) {
			exit(EXIT_FAILURE);
		}
		array_num[i].value = num;
		put_node(&array_num[0], &array_num[i], &rotate_flag);
		rotate_flag = 0;
	}

	printf("%i", check_high(&array_num[0]));
}


int main() {
	int size;
	if (scanf("%i", &size) == EOF) {
		return 0;
	}

	if (size == 0) {
		printf("0");
		return 0;
	}
	
	Tree* array_num = (Tree*)calloc(size, sizeof(Tree));

	create_avl_tree(array_num, size);

	free(array_num);
	return 0;
}
