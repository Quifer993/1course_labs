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
}LetterCode;


void delete_tree(Tree* tree) {
	if (tree->left != NULL)
		delete_tree(tree->left);

	if (tree->right != NULL)
		delete_tree(tree->right);

	if (tree != NULL) {
		free(tree);
	}
}


unsigned char bits_to_bytes(const bool bits[], int num_bits, FILE* out) {
	int num_bytes = (num_bits - 1) / 8 + 1;
	unsigned char byte = 0;
	int j = 0;
	for (j = 0; j < num_bytes - 1; j++) {
		for (int i = 0; i < 8; i++) {
			byte <<= 1;
			if ( (j * 8 + i < num_bits) && (bits[j * 8 + i]) ){
				byte |= 1;
			}
		}
		fputc(byte, out);
	}

	for (int i = 0; i < 8; i++) {
		byte <<= 1;
		if ( (j * 8 + i < num_bits) && (bits[j * 8 + i]) ) {
			byte |= 1;
		}
	}

	return byte;
}


void clear_mem(Tree** nodes, int size) {
	for (int i = size - 1; i >= 0; i--) {
		delete_tree(nodes[i]);
	}
	free(nodes);
}


Tree* make_node(Tree* left, Tree* right, int count, unsigned char byte, bool* error) {
	Tree* node = (Tree*)malloc(sizeof(Tree));
	if (node == NULL) {
		*error = true;
		return NULL;
	}
	node->left = left;
	node->right = right;
	node->count = count;
	node->byte = byte;
	return node;
}


int compare(const void* lphs, const void* rphs) {
	return (*(Tree* const*)rphs)->count - (*(Tree* const*)lphs)->count;
}


void make_alphabet(const Tree* const tree, LetterCode* codes, int height_now, char* code_mem) {
	if (tree->left != NULL) {
		code_mem[height_now] = 1;
		make_alphabet(tree->left, codes, height_now + 1, code_mem);
	}

	if (tree->right != NULL) {
		code_mem[height_now] = 0;
		make_alphabet(tree->right, codes, height_now + 1, code_mem);
	}

	if (tree->right == NULL && tree->left == NULL) {
		codes[tree->byte].size = height_now;

		for (int i = 0; i < codes[tree->byte].size; i++) {
			codes[tree->byte].code[i] = code_mem[i];
		}
	}
}


void find_leaves(const Tree* const tree, int height_now, int* byte_pos, bool bits[]) {
	if (tree->right == NULL && tree->left == NULL) {
		bits[(*byte_pos)++] = 1;

		for (int i = 0; i < 8; i++) {
			unsigned char code_bit = (tree->byte << i);
			unsigned char code = code_bit >> 7;
			bits[(*byte_pos)++] = code;
		}
		
	}
	else {
		bits[(*byte_pos)++] = 0;
		if (tree->left != NULL) {
			find_leaves(tree->left, height_now + 1, byte_pos, bits);
		}

		if (tree->right != NULL) {
			find_leaves(tree->right, height_now + 1, byte_pos, bits);
		}
	}
}


void write_text(const LetterCode codes[], FILE* in , FILE* out, int size, unsigned char last_sym_size, unsigned char last_sym, bool* error) {
	unsigned char byte = last_sym;
	int byte_pos = last_sym_size;
	byte &= 255 << (8 - last_sym_size);

	rewind(in);

	char end_of_first_line[10];
	if (fgets(end_of_first_line, 10, in) == NULL) {
		error = true;
		return;
	}

	for (int i = 0; i < size; i++) {
		unsigned char letter;
		if (fscanf(in, "%c", &letter) == EOF) {
			*error = true;
			return;
		}
		unsigned char code = 0;
		for (int j = 0; j < codes[letter].size; j++) {
			 code = codes[letter].code[j];

			byte_pos++;
			byte >>= (8 - byte_pos);
			byte |= code;
			byte <<= (8 - byte_pos);
			if (byte_pos == 8) {
				fputc(byte, out);
				byte_pos = 0;
				byte = 0;
			}
		}
	}
	fputc(byte, out);
}
 

int write_alphabet(FILE* out, const Tree* const tree, int size, unsigned char* last_sym, bool* error) {
	int size_tree = 2 * size - 1;

	bool* bits = (bool*)malloc((size_tree + size * 8) * sizeof(bool));
	if (bits == NULL) {
		*error = true;
		return 1;
		
	}

	int byte_pos = 0;
	find_leaves(tree, 1, &byte_pos, bits);

	*last_sym = bits_to_bytes(bits, byte_pos, out);

	free(bits);
	return byte_pos % 8;
}


void write_uint(unsigned int value, FILE* out) {
	unsigned int letter = 255U << 24; // 1111_1111_ and 24 '0'

	for (int i = 0; i < sizeof(value); i++) {
		fputc((value & letter) >> 8 * (3 - i), out);
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
			arr_node[j] = make_node(NULL, NULL, array_count[i], i, error);

			if (*error)
				clear_mem(arr_node, j);
				return;

			j++;
		}
	}

	qsort(arr_node, size, sizeof(arr_node[0]), compare);
	for (int j = size - 2; j >= 0; j--) {
		arr_node[j] = make_node(arr_node[j], arr_node[j + 1], arr_node[j]->count + arr_node[j + 1]->count, 'x', error);

		if (*error)
			clear_mem(arr_node, size);
			return;

		qsort(arr_node, j + 1, sizeof(arr_node[0]), compare);
	}

	LetterCode arr_code[256];
	char code[256];
	make_alphabet(arr_node[0], arr_code, 0, code);

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
	for (int i = 0; i < sizeof(*result); i++) {
		if (fscanf(in, "%c", &letter) == EOF) {
			*error = true;
			return;

		}
		*result |= (letter) << 8 * (3 - i);
	}
}


void create_tree(FILE* in, unsigned char* last_byte, unsigned int* byte_pos, Tree* node, Tree* root, bool* error) {
	if (*error)
		return;

	if (*byte_pos == 8) {
		if (fscanf(in, "%c", last_byte) == EOF) {
			delete_tree(root);
			*error = true;
			return;
			
		}
		*byte_pos = 0;
	}

	unsigned char code_bit = 0;
	code_bit = *last_byte << *byte_pos;
	code_bit >>= 7;

	(*byte_pos)++;
	if (code_bit == 0) {
		node->left = make_node(NULL, NULL, 0, '0', error);
		create_tree(in, last_byte, byte_pos, node->left, root, error);
		node->right = make_node(NULL, NULL, 0, '0', error);
		create_tree(in, last_byte, byte_pos, node->right, root, error);
	}
	else {
		unsigned char byte = 0;
		for (int i = 0; i < 8; i++) {
			if (*byte_pos == 8) {
				if (fscanf(in, "%c", last_byte) == EOF) {
					delete_tree(root);
					*error = true;
					return;
					
				}
				*byte_pos = 0;
			}
			code_bit = *last_byte << *byte_pos;
			code_bit >>= 7;
			byte <<= 1;
			byte |= code_bit;

			(*byte_pos)++;
		}
		node->byte = byte;
	}
}


void read_text_dec(FILE* in, FILE* out, unsigned char* last_byte, unsigned int* byte_pos, Tree* node, Tree* root, unsigned int* size_text, bool* error) {
	if (*error)
		return;

	if (*byte_pos == 8) {
		if (fscanf(in, "%c", last_byte) == EOF) {
			delete_tree(root);
			*error = true;
			return;
			
		}
		*byte_pos = 0;
	}

	if (node->right == NULL && node->left == NULL) {
		fputc(node->byte, out);

		(*size_text)--;
		if (*size_text == 0)
			return;
	}
	else {
		unsigned char code_bit = 0;
		code_bit = *last_byte << *byte_pos;
		code_bit >>= 7;

		(*byte_pos)++;
		if (code_bit == 1) {
			read_text_dec(in, out, last_byte, byte_pos, node->left, root, size_text, error);
		}
		else
		if (code_bit == 0) {
			read_text_dec(in, out, last_byte, byte_pos, node->right, root, size_text, error);
		}
	}
}


void decoder(FILE* in, FILE* out, bool* error) {
	unsigned int size_text = 0;
	read_uint(in, &size_text, error);
	unsigned char last_byte = 0;
	unsigned int byte_pos = 8;
	Tree *root = make_node(NULL, NULL, 0, '0', error);

	create_tree(in, &last_byte, &byte_pos, root, root, error);

	while (!(*error) && (size_text > 0) ) {
		read_text_dec(in, out, &last_byte, &byte_pos, root, root, &size_text, error);
	}

	delete_tree(root);
}


int main() {
	FILE* file_input;
	FILE* file_output;
	bool error = false;

	if ((file_input = fopen("in.txt", "rb")) == NULL){
		return 0;
	}

	if ((file_output = fopen("out.txt", "wb")) == NULL){
		fclose(file_input);
		return 0;
	}

	char end_of_first_line[10];
	if (fgets(end_of_first_line, 10, file_input) == NULL) {
		error = true;
	}
	unsigned char mode_of_work = end_of_first_line[0];

	if (!error){
		if (mode_of_work == 'c') {
			coder(file_input, file_output, &error);
		}
		else if (mode_of_work == 'd') {
			decoder(file_input, file_output, &error);
		}
	}

	fclose(file_input);
	fclose(file_output);
	return 0;
}
