#include <stdio.h>


int length_str(char text[]) {
	int length_input = 0;
	for (int i = 0; text[i] != '\0' && text[i] != EOF; i++) {
		length_input += 1;
	}

	return length_input;
}

int shift(int length_template, char text[], int shift_sym, int* number) {
	for (int i = 0; i < length_template - shift_sym; i++) {
		text[i] = text[i + shift_sym];
	}

	for (int i = length_template - shift_sym; i < length_template; i++) {
		char input_text;
		input_text = getchar();
		if (input_text == EOF) {
			return 2;
		}
		text[i] = input_text;
		*number += 1;
	}

	return 0;
}


int main() {
	char template[17]; // 16 symbols + '\0'
	char space = gets(template);
	int length_template = length_str(template);
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

	while (shift(length_template, text, shift_sym, &number) != 2) {
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

	return 0;
}
