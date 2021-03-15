#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#pragma warning(disable : 4996)


typedef struct Tree {
	struct Tree* left;
	struct Tree* right;
	int count;
	unsigned char byte;
}Tree;


Tree* make_node(Tree* left, Tree* right, int count, unsigned char byte) {
	Tree* node = (Tree*)malloc(sizeof(Tree));
	if (node == NULL) {
		exit(EXIT_FAILURE);
	}
	node->left = left;
	node->right = right;
	node->count = count;
	node->byte = byte;
	return node;
}


int compare(const void* lphs, const void* rphs) {
	int lhs = ((Tree*)lphs)->count;
	int rhs = ((Tree*)rphs)->count;

	if (lhs < rhs)
		return 1;
	else if (lhs > rhs)
		return -1;
	else
		return 0;
}


void delete_tree(Tree* tree) {
	if (tree->left != NULL)
		delete_tree(tree->left);

	if (tree->right != NULL)
		delete_tree(tree->right);
	free(tree);
}


void find_lists(Tree* tree, unsigned char* bits, int* high_now) {

	if (tree->left != NULL) {
		*bits |= 1 << (8 - *high_now);
		*high_now += 1;
		find_lists(tree->left, bits, high_now);
	}

	if (tree->right != NULL) {
		unsigned char mask = 255 << (9 - *high_now);
		*bits &= mask;
		*high_now += 1;
		find_lists(tree->right, bits, high_now);
	}
	*high_now -= 1;

	if (tree->right == NULL && tree->left == NULL) {
		unsigned char code_bit = 0;
		unsigned char code = 0;
		printf("%c: ", tree->byte);
		for (int i = 0; i < *high_now; i++) {
			code = *bits << i;
			code_bit = code >> 7;

			printf("%i", code_bit);
		}
		printf("\n");
	}
}


void coder(FILE* in, FILE* out) {
	int array_count[256] = {0};
	unsigned char symbol;
	while (fscanf(in, "%c", &symbol) != EOF) {
		array_count[symbol]++;
	}

	int size = 0;
	for (int i = 0; i < 256; i++) {
		if (array_count[i] != 0) {
			size++;
		}
	}

	Tree** arr_node = (Tree**)calloc(size, sizeof(Tree*));
	int j = 0;
	for (int i = 0; i < 256; i++) {
		if (array_count[i] != 0) {
			arr_node[j] = make_node(NULL, NULL, array_count[i], i);
			j++;
		}
	}
	
//	for (int i = 0; i < size; i++) {
//		printf("%c -- %i\n", arr_node[i]->byte, arr_node[i]->count);
//	}

	qsort(arr_node, size, sizeof(arr_node[0]), compare);
	/*printf("\n");
	for (int i = 0; i < size; i++) {
		printf("%c -- %i\n", arr_node[i]->byte, arr_node[i]->count);
	}*/


	for (int j = size - 1; j > 1; j--) {
		arr_node[j - 1]  = make_node(arr_node[j - 1], arr_node[j - 2], arr_node[j - 1]->count + arr_node[j - 2]->count, ' ');
		qsort(arr_node, size , sizeof(arr_node[0]), compare);
		for (int i = 0; i < j; i++) {
			printf("%c -- %i\n", arr_node[i]->byte, arr_node[i]->count);
		}
		printf("\n");
	}

//	printf("\n%i\n",arr_node[0]->count);

	int flag = 0;
	unsigned char bits = 0;
	int high_now = 1;
	find_lists(arr_node[0], &bits, &high_now);


	delete_tree(arr_node[0]);
	free(arr_node);
}


void decoder() {

}


int main() {
	FILE* file_input;
	FILE* file_output;

	if ((file_input = fopen("in.txt", "r")) == NULL){
		printf("Error: file not open");
		return 0;
	}

	if ((file_output = fopen("out.txt", "w")) == NULL){
		printf("Error: file not open");
		return 0;
	}

	unsigned char mode_of_work;
	if (fscanf(file_input, "%c", &mode_of_work) == EOF) {
		return 0;
	}

	fgetc(file_input);
	
	if (mode_of_work == 'c') {
		coder(file_input, file_output);
	}
	else if (mode_of_work == 'd') {
		decoder();
	}

	fclose(file_input);
	fclose(file_output);
	return 0;
}