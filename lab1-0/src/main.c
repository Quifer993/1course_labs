#include <stdio.h>


int shift(int length_template, char text[], int shift_sym, FILE* in) {
	if (shift_sym != length_template) {
		int begin = length_template - shift_sym;
		for (int i = 0; i < begin; i++) {
			text[i] = text[i + shift_sym];
		}

		for (int i = begin; i < length_template; i++) {
			if (fscanf(in, "%c", &text[i]) == 0) {
				return 2;
			}

		}
	}
	else {
		if (!fread(text, 1, length_template, in) || feof(in)) {
			return 2;
		}

	}


	return 0;
}


int main() {
	FILE* in = fopen("in.txt", "rt");
	if (in == NULL) return 0;
	char template[18]; // 16 symbols + '\0'
	int length_template = 0;
	for (int i = 0; i < 17; i++) {
		if (fscanf(in, "%c", &template[i]) == 0) {
			fclose(in);
			return 0;
		}
		if (template[i] == '\n') {
			template[i] = '\0';
			break;
		}
		length_template += 1;
	}
	int last_symbol = length_template - 1;

	int repit_last_symbol = length_template;
	for (int i = last_symbol - 1; i >= 0; i--) {
		if (template[last_symbol] == template[i]) {
			repit_last_symbol = last_symbol - i;
			break;
		}
	}

	int number = 0;
	int shift_sym = length_template;
	char text[17];

	while (shift(length_template, text, shift_sym, in) != 2) {
		number += shift_sym;
		if (feof(in)) {
			fclose(in);
			return 0;
		}
		printf("%i ", number);
		if (text[last_symbol] == template[last_symbol]) {
			int k;
			for (k = last_symbol - 1; k >= 0; k--) {
				printf("%i ", number - last_symbol + k);
				if (template[k] != text[k]) {
					shift_sym = repit_last_symbol;
					break;
				}
			}
			if (k == -1) {
				shift_sym = repit_last_symbol;
			}
		}
		else {
			int is_exist_in_input = -1;
			int j;
			for (j = last_symbol - 1; j >= 0; j--) {
				if (text[last_symbol] == template[j]) {
					is_exist_in_input = j;
					shift_sym = length_template - j - 1;
					break;
				}

			}
			if (is_exist_in_input < 0) {
				shift_sym = length_template;
			}
		}
	}

	fclose(in);
	return 0;
}
