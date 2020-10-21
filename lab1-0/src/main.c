#include <stdio.h>


int shift(int length_template, char text[], int shift_sym, int* number, FILE* in) {
	if (shift_sym != length_template) {
		for (int i = 0; i < length_template - shift_sym; i++) {
			text[i] = text[i + shift_sym];
		}

		for (int i = length_template - shift_sym; i < length_template; i++) {
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

	int repit_last_symbol = length_template;
	for (int i = length_template - 2; i >= 0; i--) {
		if (template[length_template - 1] == template[i]) {
			repit_last_symbol = length_template - i - 1;
			break;
		}
	}

	int number = 0;
	int shift_sym = length_template;
	char text[17];

	while (shift(length_template, text, shift_sym, &number, in) != 2) {
		number += shift_sym;
		if (feof(in)) {
			fclose(in);
			return 0;
		}
		printf("%i ", number);
		if (text[length_template - 1] == template[length_template - 1]) {
			int k;
			for (k = length_template - 2; k >= 0; k--) {
				printf("%i ", number - length_template + k + 1);
				if (template[k] != text[k]) {
					shift_sym = repit_last_symbol;
					break;
				}
			}
			if (k == -1) {
				shift_sym = length_template;
			}
		}
		else {
			int is_exist_in_input = -1;
			int j;
			for (j = length_template - 2; j >= 0; j--) {
				if (text[length_template - 1] == template[j]) {
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
