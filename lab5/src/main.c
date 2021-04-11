#include <stdio.h>
#include <malloc.h>
#include <stdbool.h>
#include <stdlib.h>


typedef struct Tree {
	int count;
	unsigned char byte;
	struct Tree* left;
	struct Tree* right;
}Tree;


typedef struct Letter_code {
	unsigned char code[256];
	int size;
}Letter_code;


void delete_tree(Tree* tree) {
	if (tree->left != NULL)
		delete_tree(tree->left);

	if (tree->right != NULL)
		delete_tree(tree->right);

	if (tree != NULL)
		free(tree);

}


unsigned char bits_to_bytes(const bool bits[], int num_bits, FILE* out) {
	int num_bytes = (num_bits - 1) / 8 + 1;
	unsigned char byte = 0;
	int j = 0;
	for (j = 0; j < num_bytes - 1; j++) {
		for (int i = 0; i < 8; i++) {

			byte = byte << 1;
			if (j * 8 + i < num_bits) {
				if (bits[j * 8 + i]) {
					byte = byte | 1;
				}
			}
			
		}
		fprintf(out, "%c", byte);
	}

	for (int i = 0; i < 8; i++) {

		byte = byte << 1;
		if (j * 8 + i < num_bits) {
			if (bits[j * 8 + i]) {
				byte = byte | 1;
			}
		}

	}

	return byte;
}


Tree* make_node(Tree** arr_node, Tree* left, Tree* right, int count, unsigned char byte, int size, bool* error) {
	Tree* node = (Tree*)malloc(sizeof(Tree));
	if (node == NULL) {
		for (int i = size - 1; i >= 0; i-- ) {
			delete_tree(arr_node[i]);
		}
		free(arr_node);
		*error = true;
		return NULL;
		//close_file(in, out , EXIT_FAILURE);
	}
	node->left = left;
	node->right = right;
	node->count = count;
	node->byte = byte;
	return node;
}


Tree* make_node_dec(Tree* left, Tree* right, int count, unsigned char byte, bool* error) {
	Tree* node = (Tree*)malloc(sizeof(Tree));
	if (node == NULL) {
		*error = true;
		return NULL;
		//close_file(in, out , EXIT_FAILURE);
	}
	node->left = left;
	node->right = right;
	node->count = count;
	node->byte = byte;
	return node;
}


int compare(const void* lphs, const void* rphs) {
	int lhs = (*(Tree* const*)lphs)->count;
	int rhs = (*(Tree* const*)rphs)->count;

	if (lhs < rhs)
		return 1;
	else if (lhs > rhs)
		return -1;
	else
		return 0;
}


void make_alphabet(Tree* tree, int* high_now, Letter_code* arr_code, char* code_mem) {

	if (tree->left != NULL) {
		code_mem[*high_now] = 1;
		*high_now += 1;
		make_alphabet(tree->left, high_now, arr_code, code_mem);
	}

	if (tree->right != NULL) {
		code_mem[*high_now] = 0;
		*high_now += 1;
		make_alphabet(tree->right, high_now, arr_code, code_mem);
	}
	*high_now -= 1;

	if (tree->right == NULL && tree->left == NULL) {
		arr_code[tree->byte].size = *high_now + 1;

		for (int i = 0; i < arr_code[tree->byte].size; i++) {
			arr_code[tree->byte].code[i] = code_mem[i];
		}
	}

}


void find_lists(Tree* tree, int* high_now, bool bits[], int* point ) {

	if (tree->right == NULL && tree->left == NULL) {
		bits[(*point)++] = 1;

		//printf("%c: ", tree->byte);
		for (int i = 0; i < 8; i++) {
			unsigned char code_bit = (tree->byte << i);
			unsigned char code = code_bit >> 7;

			bits[(*point)++] = code;
			//printf("%i", bits[(*point) - 1]);
		}
		//printf("\n");
		
	}
	else {
		bits[(*point)++] = 0;
		if (tree->left != NULL) {
			*high_now += 1;
			find_lists(tree->left, high_now, bits, point);
		}

		if (tree->right != NULL) {
			*high_now += 1;
			find_lists(tree->right, high_now, bits, point);
		}
	}

	*high_now -= 1;


//	printf("\n");
//	for (int i = 0; i < 69; i++) {
//		printf("%i", bits[i]);
//	}
//	printf("\n\n");

}


void write_text(Letter_code arr_code[], FILE* in , FILE* out, int size, unsigned char last_sym_size, unsigned char last_sym, bool* error) {
	unsigned char byte = last_sym;
	int point = last_sym_size;
	byte &= 255 << (8 - last_sym_size);

	rewind(in);
	unsigned char letter;

	char useless2[10];
	if (fgets(useless2, 10, in) == 0) {
		*error = true;
		return;
	}

	unsigned char code = 0;

	for (int i = 0; i < size; i++) {

		if (fscanf(in, "%c", &letter) == EOF) {
			*error = true;
			return;
			
		}
		for (int j = 0; j < arr_code[letter].size; j++) {
			code = arr_code[letter].code[j];


			//printf("%i", code);
			point++;
			byte >>= (8 - point);
			byte |= code;
			byte <<= (8 - point);
			if (point == 8) {
				//fprintf(out, "%c", n, name);
				//printf("%i\n", byte);
				//printf("\t");
				fputc(byte, out);
				point = 0;
				byte = 0;
			}
		}
	}
	fputc(byte, out);

}


int write_alphabet(FILE* out, Tree* node, int size, unsigned char* last_sym, bool* error) {
	int high_now = 1;
	int size_tree = 2 * size - 1;

	bool* bits = (bool*)malloc((size_tree + size * 8) * sizeof(bool));
	if (bits == NULL) {
		*error = true;
		return EXIT_FAILURE;
		
	}

	int point = 0;
	find_lists(node, &high_now, bits, &point);

	*last_sym = bits_to_bytes(bits, point, out);
	int mod = point - point / 8 * 8;

	free(bits);
	if (mod != 0) {
		return mod;
	}

	return 0;
}


void write_uint(unsigned int value, FILE* out) {
	unsigned int letter = 255 << 24; // 1111_1111_ and 24 '0'

	for (int i = 0; i < 4; i++) {
		fprintf(out, "%c", (value & letter) >> 8 * (3 - i));
		letter >>= 8;
	}
}


void coder(FILE* in, FILE* out, bool* error) {
	int array_count[256] = { 0 };
	unsigned char symbol;
	unsigned int size_text = 0;
	while (fscanf(in, "%c", &symbol) != EOF) {
		array_count[symbol]++;
		size_text++;
	}
		
	if (size_text == 0)
		return;

	int size = 0;
	for (int i = 0; i < 256; i++) {
		if (array_count[i] != 0) {
			size++;
		}
	}

	Tree** arr_node = (Tree**)calloc(size, sizeof(Tree*));
	if (arr_node == NULL) {
		*error = true;
		return;
	}

	int j = 0;
	for (int i = 0; i < 256; i++) {
		if (array_count[i] != 0) {
			arr_node[j] = make_node(arr_node, NULL, NULL, array_count[i], i, j, error);

			if (*error)
				return;

			j++;
		}
	}


	qsort(arr_node, size, sizeof(arr_node[0]), compare);

	for (int j = size - 2; j >= 0; j--) {
		arr_node[j] = make_node(arr_node, arr_node[j], arr_node[j + 1], arr_node[j]->count + arr_node[j + 1]->count, 'x', size, error);

		if (*error)
			return;

		qsort(arr_node, j + 1, sizeof(arr_node[0]), compare);
	}

	int high_now = 0;
	Letter_code arr_code[256];
	/*for (int i = 0; i < 256; i++) {
		arr_code[i].code = (unsigned char*)malloc(255 * sizeof(unsigned char));
		if (arr_code[i].code == NULL) {
			for (int j = 0; j < i; j++) {
				free(arr_code[j].code);
			}
			free(arr_node);
			*error = true;
			return;
			
		}
	}*/
	char code[256];
	make_alphabet(arr_node[0], &high_now, arr_code, code);

	write_uint(size_text, out);

	unsigned char last_sym;
	unsigned char last_symbol_size = write_alphabet(out, arr_node[0], size, &last_sym, error);

	delete_tree(arr_node[0]);
	free(arr_node);
	if (*error)
		return;


	write_text(arr_code, in, out, size_text, last_symbol_size, last_sym, error);
}


//decoder fun
void read_uint(FILE* in, unsigned int* result, bool* error) {
	unsigned char letter = 0;
	for (int i = 0; i < 4; i++) {
		if (fscanf(in, "%c", &letter) == EOF) {
			*error = true;
			return;
			
		}

		*result |= (letter) << 8 * (3 - i);
	}

}


void create_tree(FILE* in, unsigned char* last_byte, unsigned int* point, Tree* node, Tree* root, bool* error) {
	if (*error) {
		return;
	}

	if (*point == 8) {
		if (fscanf(in, "%c", last_byte) == EOF) {
			delete_tree(root);
			*error = true;
			return;
			
		}
		//printf("%i\n", *last_byte);
		*point = 0;
	}//сделать функцию считывания символа

	unsigned char code_bit = 0;
	code_bit = *last_byte << *point;
	code_bit = code_bit >> 7;

	(*point)++;
	if (code_bit == 0) {
		node->left = make_node_dec(NULL, NULL, 0, '0', error);
		create_tree(in, last_byte, point, node->left, root, error);
		node->right = make_node_dec(NULL, NULL, 0, '0', error);
		create_tree(in, last_byte, point, node->right, root, error);
	}
	else {
		unsigned char byte = 0;
		for (int i = 0; i < 8; i++) {
			if (*point == 8) {
				if (fscanf(in, "%c", last_byte) == EOF) {
					delete_tree(root);
					*error = true;
					return;
					
				}
				//printf("%i\n", *last_byte);
				*point = 0;
			}
			code_bit = *last_byte << *point;
			code_bit = code_bit >> 7;

			byte <<= 1;
			byte |= code_bit;

			(*point)++;

		}
		node->byte = byte;
	}

}


void read_text_dec(FILE* in, FILE* out, unsigned char* last_byte, unsigned int* point, Tree* node, Tree* root, unsigned int* size_text, bool* error) {
	if (*error) {
		return;
	}

	if (*point == 8) {
		if (fscanf(in, "%c", last_byte) == EOF) {
			delete_tree(root);
			*error = true;
			return;
			
		}
		*point = 0;
	}//сделать функцию считывания символа

	if (node->right == NULL && node->left == NULL) {
		fprintf(out, "%c", node->byte);
		//printf("%c", node->byte);
		(*size_text)--;
		if (*size_text == 0) {
			return;
		}
	}
	else {
		unsigned char code_bit = 0;
		code_bit = *last_byte << *point;
		code_bit = code_bit >> 7;

		(*point)++;
		if (code_bit == 1) {
			read_text_dec(in, out, last_byte, point, node->left, root, size_text, error);
		}
		else
		if (code_bit == 0) {
			read_text_dec(in, out, last_byte, point, node->right, root, size_text, error);
		}
	}
}


void decoder(FILE* in, FILE* out, bool* error) {
	unsigned int size_text = 0;
	read_uint(in, &size_text, error);
	unsigned char last_byte = 0;
	unsigned int point = 8;
	Tree *root = make_node_dec(NULL, NULL, 0, '0', error);

	if (*error) {
		return;
	}
	//node = make_node(NULL, NULL, 0, '0'); //(Tree*)malloc(sizeof(Tree));     //Tree* branch = (Tree*)malloc(sizeof(Tree));
	create_tree(in, &last_byte, &point, root, root, error);

	while (*error && (size_text > 0) ) {
		read_text_dec(in, out, &last_byte, &point, root, root, &size_text, error);
	}

	delete_tree(root);
}


int main() {
	FILE* file_input;
	FILE* file_output;
	bool error = 0;


	if ((file_input = fopen("in.txt", "rb+")) == NULL){
		return 0;
	}

	if ((file_output = fopen("out.txt", "wb")) == NULL){
		fclose(file_input);
		return 0;
	}

	unsigned char mode_of_work;
	if (fscanf(file_input, "%c", &mode_of_work) == EOF) {
		//close_file(file_input, file_output, EXIT_FAILURE);
		//return 0;
		error = 1;
	}

	char useless2[10];
	if (!error && fgets(useless2, 10, file_input) != NULL) {
		if (mode_of_work == 'c') {
			coder(file_input, file_output, &error);
		}
		else if (mode_of_work == 'd') {
			decoder(file_input, file_output, &error);
		}//////////////////пустые файлы обработать
	}

	fclose(file_input);
	fclose(file_output);
	//close_file(file_input, file_output, EXIT_SUCCESS);
	if (error == 1) {
		return 1;
	}
	return 0;
}
