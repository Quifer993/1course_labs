#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
#pragma warning(disable : 4996)


typedef struct Tree {
	struct Tree* left;
	struct Tree* right;
	int value;
	int height;
}Tree;


Tree* make_node(Tree* left, Tree* right, int value, int height) {
	Tree* node = (Tree*)malloc(sizeof(Tree));
	if (node == NULL) {
		exit(EXIT_FAILURE);
	}
	node->left = left;
	node->right = right;
	node->value = value;
	node->height = height;
	return node;
}


void delete_tree(Tree* tree) {
	if (tree->left != NULL)
		delete_tree(tree->left);

	if (tree->right != NULL)
		delete_tree(tree->right);
	free(tree);
}



void small_rotate(Tree** node) {

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
		return maximum(check_high(tree->left), check_high(tree->right) + 1);
	}
	
	return 0;

	/*if (tree->left != NULL) {
		*high_now += 1;
		check_high(tree->left, high_max, high_now);
	}

	if (tree->right != NULL) {
		*high_now += 1;
		check_high(tree->right, high_max, high_now);
	}

	if (*high_max < *high_now) {
		*high_max = *high_now;
	}

	*high_now -= 1;
	*/
}


void check_root(Tree* node, int num, int height) {
	if (node->value < num) {
		if (node->right == NULL) {
			node->right = make_node(NULL, NULL, num, height + 1);
		}
		else {
			check_root(node->right, num, height + 1);
		}
	}
	else {
		if (node->left == NULL) {
			node->left = make_node(NULL, NULL, num, height + 1);
		}
		else {
			check_root(node->left, num, height + 1);
		}
	}

	if (check_high(node->left) - check_high(node->right) > 1) {
		printf("2");
	}

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
		

	int num;
	Tree* root = make_node(NULL, NULL, 0, 1);
	if (scanf("%i", &num) != EOF) {
		root->value = num;
	}
	else {
		return 0;
	}

	int  height = 1;
	while (scanf("%i", &num) != EOF) {
		check_root(root, num, height); 

		//Tree* node= make_node(NULL, NULL, 0, 1);
	}

	int high_now = 1;
	int high_max = 1;
	high_max = check_high(root);
	printf("%i", high_max);
	/*int i = 0;
	while (size >= pow(2, i)) {
		i++;
	}
	printf("%i", i);*/
	delete_tree(root);
}