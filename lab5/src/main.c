#include <stdio.h>
#include <malloc.h>
#include <stdbool.h>
#include <stdlib.h>
#pragma warning(disable : 4996)


typedef struct Tree {
	int count;
	unsigned char byte;
	struct Tree* left;
	struct Tree* right;
}Tree;


typedef struct Letter_code {
	unsigned char code;
	int size;
}Letter_code;


void close_file(FILE* in, FILE* out, int error) {
	fclose(in);
	fclose(out);
	if (error != 0) {
		exit(EXIT_FAILURE);
	}
	else {
		exit(EXIT_SUCCESS);
	}
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


Tree* make_node(FILE* in, FILE* out, Tree* left, Tree* right, int count, unsigned char byte) {
	Tree* node = (Tree*)malloc(sizeof(Tree));
	if (node == NULL) {
		close_file(in, out , EXIT_FAILURE);
	}
	node->left = left;
	node->right = right;
	node->count = count;
	node->byte = byte;
	return node;
}


int compare(const void* lphs, const void* rphs) {
	int lhs = (*(Tree**)lphs)->count;
	int rhs = (*(Tree**)rphs)->count;

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


void make_alphabet(Tree* tree, unsigned char* bits, int* high_now, Letter_code arr_code[]) {

	if (tree->left != NULL) {
		*bits |= 1 << (8 - *high_now);
		*high_now += 1;
		make_alphabet(tree->left, bits, high_now, arr_code);
	}

	if (tree->right != NULL) {
		unsigned char mask = 255 << (9 - *high_now);
		*bits &= mask;
		*high_now += 1;
		make_alphabet(tree->right, bits, high_now, arr_code);
	}
	*high_now -= 1;

	if (tree->right == NULL && tree->left == NULL) {
		arr_code[tree->byte].size = *high_now;
		arr_code[tree->byte].code = *bits;
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


void write_text(Letter_code arr_code[], FILE* in , FILE* out, int size, unsigned char last_sym_size, unsigned char last_sym) {
	unsigned char byte = last_sym;
	int point = last_sym_size;
	byte &= 255 << (8 - last_sym_size);

	rewind(in);
	char useless = 0;
	unsigned char letter;
	while (useless != '\n') {
		useless = fgetc(in);
		if (feof(in) != 0) {
			close_file(in, out, EXIT_FAILURE);
		}
	}

	unsigned char code = 0;
	unsigned char code1 = 0;

	for (int i = 0; i < size; i++) {
		
		if (fscanf(in, "%c", &letter) == EOF) {
			close_file(in, out, EXIT_FAILURE);
		}
		for (int j = 0; j < arr_code[letter].size; j++) {
			code = arr_code[letter].code << j;
			code1 = code >> 7;
			//printf("%i", code1);
			point++;
			byte >>= (8 - point);
			byte |= code1;
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
	//if (point != 0) {
		fputc(byte, out);
	//}
	//printf("\n");
}


int write_alphabet(Tree* node , FILE* in, FILE* out, int size, unsigned char* last_sym) {

	int high_now = 1;
	int pow = 0;
	for (int i = 0; (1 << pow) < size; i++) {
		pow++;
	}
	int size_tree = 2 * size - 1;// size - ((1 << pow) - size) * 2 + (1 << pow);
	bool* bits = (bool*)malloc((size_tree + size * 8) * sizeof(bool));
	if (bits == NULL) {
		close_file(in, out, EXIT_FAILURE);
	}
	int point = 0;
	find_lists(node, &high_now, bits, &point);
	//unsigned char* bytes = (char*)malloc((size_tree - 1) / 8 * sizeof(char));
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


void coder(FILE* in, FILE* out) {
	int array_count[256] = { 0 };
	unsigned char symbol;
	unsigned int size_text = 0;
	while (fscanf(in, "%c", &symbol) != EOF) {
		array_count[symbol]++;
		size_text++;
	}
		
	if (size_text == 0) {
		close_file(in, out, EXIT_SUCCESS);
	}

	int size = 0;
	for (int i = 0; i < 256; i++) {
		if (array_count[i] != 0) {
			size++;
		}
	}

	Tree** arr_node = (Tree**)calloc(size, sizeof(Tree*));
	if (arr_node == NULL) {
		close_file(in, out, EXIT_FAILURE);
	}
	int j = 0;
	for (int i = 0; i < 256; i++) {
		if (array_count[i] != 0) {
			arr_node[j] = make_node(in, out, NULL, NULL, array_count[i], i);
			j++;
		}
	}


	qsort(arr_node, size, sizeof(arr_node[0]), compare);

	for (int j = size - 2; j >= 0; j--) {
		arr_node[j] = make_node(in, out, arr_node[j], arr_node[j + 1], arr_node[j]->count + arr_node[j + 1]->count, 'x');
		qsort(arr_node, j + 1, sizeof(arr_node[0]), compare);
	}

	unsigned char bits = 0;
	int high_now = 1;
	Letter_code arr_code[256];
	make_alphabet(arr_node[0], &bits, &high_now, arr_code);

	write_uint(size_text, out);

	unsigned char last_sym;
	unsigned char last_symbol_size = write_alphabet(in, out, arr_node[0], out, size, &last_sym);

	write_text(arr_code, in, out, size_text, last_symbol_size, last_sym);

	delete_tree(arr_node[0]);
	free(arr_node);
}


//decoder fun
void read_uint(FILE* in, FILE* out, unsigned int* result) {
	unsigned char letter = 0;
	for (int i = 0; i < 4; i++) {
		if (fscanf(in, "%c", &letter) == EOF) {
			close_file(in, out, EXIT_FAILURE);
		}

		*result |= (letter) << 8 * (3 - i);
	}

}


void create_tree(FILE* in, FILE* out, unsigned char* last_byte, unsigned int* point, Tree* node) {
		if (*point == 8) {
			if (fscanf(in, "%c", last_byte) == EOF) {
				close_file(in, out, EXIT_FAILURE);
			}
			//printf("%i\n", *last_byte);
			*point = 0;
		}//сделать функцию считывания символа
		
		unsigned char code_bit = 0;
		code_bit = *last_byte << *point;
		code_bit = code_bit >> 7;

		(*point)++;
		if (code_bit == 0) {
			//create_node(node);
			node->left = make_node(in, out, NULL, NULL, 0, '0');
			create_tree(in, out, last_byte, point, node->left);
			node->right = make_node(in, out, NULL, NULL, 0, '0');
			create_tree(in, out, last_byte, point, node->right);
		}
		else {
			unsigned char byte = 0;
			for (int i = 0; i < 8; i++) {
				if (*point == 8) {
					if (fscanf(in, "%c", last_byte) == EOF) {
						close_file(in, out, EXIT_FAILURE);
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
			//	= make_node(NULL, NULL, 0, byte);


		}
}


void read_text_dec(FILE* in, FILE* out, unsigned char* last_byte, unsigned int* point, Tree* node, unsigned int* size_text) {
	if (*point == 8) {
		if (fscanf(in, "%c", last_byte) == EOF) {
			close_file(in, out, EXIT_FAILURE);
		}
		//printf("%i", *last_byte);
		*point = 0;
	}//сделать функцию считывания символа



	if (node->right == NULL && node->left == NULL) {
		fprintf(out, "%c", node->byte);
		//printf("%c", node->byte);
		(*size_text)--;
		if (*size_text == 0) {
			close_file(in, out,EXIT_SUCCESS);
		}
	}
	else {
		unsigned char code_bit = 0;
		code_bit = *last_byte << *point;
		code_bit = code_bit >> 7;

		(*point)++;
		if (code_bit == 1) {
			read_text_dec(in, out, last_byte, point, node->left, size_text);
		}
		else
		if (code_bit == 0) {
			read_text_dec(in, out, last_byte, point, node->right, size_text);
		}
	}


}


void decoder(FILE* in, FILE* out) {
	unsigned int size_text = 0;
	read_uint(in, out, &size_text);

	unsigned char last_byte = 0;
	unsigned int point = 8;
	Tree *root = make_node(in, out, NULL, NULL, 0, '0');
	//node = make_node(NULL, NULL, 0, '0'); //(Tree*)malloc(sizeof(Tree));     //Tree* branch = (Tree*)malloc(sizeof(Tree));
	create_tree(in, out, &last_byte, &point, root);
	while (size_text > 0) {
		read_text_dec(in, out, &last_byte, &point, root, &size_text);
	}

	free(root);
}


int main() {
	FILE* file_input;
	FILE* file_output;

	if ((file_input = fopen("in.txt", "rb+")) == NULL){
		return 0;
	}

	if ((file_output = fopen("out.txt", "wb+")) == NULL){
		fclose(file_input);
		return 0;
	}

	unsigned char mode_of_work;
	if (fscanf(file_input, "%c", &mode_of_work) == EOF) {
		close_file(file_input, file_output, EXIT_FAILURE);
		return 0;
	}

	fgetc(file_input);
	
	if (mode_of_work == 'c') {
		coder(file_input, file_output);
	}
	else if (mode_of_work == 'd') {
		decoder(file_input, file_output);
	}//////////////////пустые файлы обработать

	close_file(file_input, file_output, EXIT_SUCCESS);
	return 0;
}
