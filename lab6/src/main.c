#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>


typedef struct Node {
	struct Node* left;
	struct Node* right;
	int value;
	int height;
}Node;


typedef struct Tree {
	struct Node* root;
}Tree;


int maximum(int a, int b) {
	if (a > b) {
		return a;
	}
	else {
		return b;
	}
}


int fix_height(const Node* node) {
	if (node->right != NULL && node->left != NULL) {
		return maximum(node->left->height, node->right->height) + 1;
	}
	else if (node->right == NULL && node->left == NULL) {
		return 1;
	}
	else if (node->left != NULL) {
		return  node->left->height + 1;
	}
	else {
		return  node->right->height + 1;
	}

	
}


Node* small_rotate_left(Node* root) {
	Node* new_root = root->right;
	root->right = new_root->left;
	new_root->left = root;

	root->height = fix_height(root);
	new_root->height = fix_height(new_root);
	
	return new_root;
}


Node* small_rotate_right(Node* root) {
	Node* new_root = root->left;
	root->left = new_root->right;
	new_root->right = root;

	root->height = fix_height(root);
	new_root->height = fix_height(new_root);

	return new_root;
}


int check_balance(const Node* node) {
	//if (node == NULL) {
	//	return 0;
	//}
	if ( node->left == NULL && node->right == NULL) {
		return 0;
	}
	if (node->left == NULL) {
		return node->right->height;
	}
	else if (node->right == NULL) {
		return -(node->left->height);
	}
	else {
		return node->right->height - node->left->height;
	}
}


Node* use_rotate(Node* node) {
	int balance = 0;
	if (node != NULL) {
		balance = check_balance(node);
	}


	if (balance == 2) {
		if (check_balance(node->right) == -1) {
			node->right = small_rotate_right(node->right);
		}
		node = small_rotate_left(node);
	}
	if (balance == -2) {
		if (check_balance(node->left) == 1) {
			node->left = small_rotate_left(node->left);
		}
		node = small_rotate_right(node);
	}

	return node;
}


Node* put_node(Node* root, Node* node) {
	if (root == NULL) {
		node->height = 1;
		return node;
	}
	else if (root->value >= node->value) {
		if (root->left != NULL) {
			root->left = put_node(root->left, node);
			root->left = use_rotate(root->left);

		}
		else { 
			root->left = node;
		}
	}
	else {
		if (root->right != NULL) {
			root->right = put_node(root->right, node);
			root->right = use_rotate(root->right);
		}
		else {
			root->right = node;
		}
	}

	root->height = fix_height(root);
	return root;
}


int create_avl_tree(FILE* input_file, int size, Node* array_num, Tree* tree) {
	int num;

	for (int i = 0; i < size; i++) {
		
		if(fscanf(input_file, "%i", &num) == EOF) {
			return EXIT_FAILURE;
		}
		array_num[i].left = NULL;
		array_num[i].right = NULL;
		array_num[i].height = 1;
		array_num[i].value = num;
		tree->root = put_node(tree->root, &array_num[i]);
		tree->root = use_rotate(tree->root);
	}

	return EXIT_SUCCESS;

}


int main() {
	FILE* input_file;
	FILE* output_file;

	if ((input_file = fopen("in.txt", "r")) == NULL) {
		return EXIT_SUCCESS;
	}

	if ((output_file = fopen("out.txt", "w")) == NULL) {
		fclose(input_file);
		return EXIT_SUCCESS;
	}

	int size;
	if (fscanf( input_file, "%i", &size ) == EOF) {
		fclose(input_file);
		fclose(output_file);
		return EXIT_SUCCESS;
	}

	if (size == 0) {
		fputc('0', output_file);
	}
	else {
		Node* array_num = (Node*)malloc(size * sizeof(Node));
		if (array_num == NULL) {
			fclose(input_file);
			fclose(output_file);
			return EXIT_FAILURE;
		}

		Tree tree;
		tree.root = NULL;
		if (create_avl_tree(input_file, size, array_num, &tree) == EXIT_SUCCESS) {
			fprintf(output_file, "%i", tree.root->height);
		}

		free(array_num);
	}

	fclose(input_file);
	fclose(output_file);
	return EXIT_SUCCESS;
}
