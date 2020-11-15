#include <stdio.h>
#include <math.h>

unsigned int make_hash(const unsigned char text[], long long int* power, const int length) {
	unsigned int hash = 0;

	for (int i = 0; i < length; i++) {
		hash += (text[i] % 3) * *power;
		*power *= 3;
	}
	*power /= 3;

	return hash;
}

int print_result(const int text_pos, const int length_template, const unsigned char* template, const unsigned char* text) {
	for (int i = 0; i < length_template; i++) {
		printf("%d ", text_pos - length_template + i + 1);
		if (template[i] != text[(text_pos + i) % length_template]) {
			break;
		}
	}

	return 0;
}


int main() {
	FILE* in = fopen("in.txt", "rt");
	if (in == NULL) return 0;

	unsigned char template[17];
	int length_template = 0;
	for (int i = 0; i < 17; i++) {
		if (fscanf(in, "%c", &template[i]) == EOF) {
			fclose(in);
			return 0;
		}
		if (template[i] == '\n') {
			template[i] = '\0';
			break;
		}
		length_template += 1;
	}

	long long int power = 1;
	const unsigned int hash_template = make_hash(template, &power, length_template);
	printf("%u ", hash_template);

	unsigned char text[17];
	int text_position = length_template;
	for (int i = 0; i < length_template; i++) {
		if (fscanf(in, "%c", &text[i]) == EOF) {
			fclose(in);
			return 0;
		}
	}

	power = 1;
	unsigned int hash_string = make_hash(text, &power, length_template);

	if (hash_string == hash_template) {
		print_result(text_position, length_template, template, text);
	}
	while (fscanf(in, "%c", &text[text_position % length_template]) != EOF) {
		hash_string = hash_string / 3 + power * (text[text_position % length_template] % 3);
		text_position += 1;
		if (hash_string == hash_template) {
			print_result(text_position, length_template, template, text);
		}
	}

	fclose(in);
	return 0;
}
