#include <stdio.h>
#include <math.h>
//#pragma warning(disable : 4996)


int main() {
	FILE* in = fopen("in.txt", "rt");
	if (in == NULL) return 0;
	unsigned char template[17];
	int length_template = 0;
	int ht = 0; //hash template
	long long int power = 1;
	for (int i = 0; i < 17; i++) {
		if (fscanf(in, "%c", &template[i]) == 0) {
			fclose(in);
			return 0;
		}
		if (template[i] == '\n') {
			template[i] = '\0';
			break;
		}
		template[i] = template[i] % 3;
		ht += template[i] * power;
		power *= 3;
		length_template += 1;
	}
	printf("%d ", ht);

	unsigned char text[17];
	int hs = 0;
	power = 1;
	int number = length_template;
	for (int i = 0; i < length_template; i++) {
		if (fscanf(in, "%c", &text[i]) == EOF) {
			fclose(in);
			return 0;
		}
		text[i] = text[i] % 3;
		hs += text[i] * power;
		power *= 3;
	}
	power /= 3;

	if (hs == ht) {
		for (int i = length_template - 1; i >= 0; i--) {
			printf("%d ", number - i);
		}
	}
	unsigned char input_char;
	while (fscanf(in, "%c", &input_char) != EOF) {
		input_char = input_char % 3;
		unsigned char first = text[number % length_template];
		hs = (hs - first)/3 + power * input_char;
		text[number % length_template] = input_char;
		number += 1;
		if (hs == ht) {
			for (int i = length_template -1; i >= 0; i--) {
				printf("%d ", number - i);
			}
		}
		//printf("%d ", hs);
	}


	int last_symbol = length_template - 1;
	fclose(in);
	return 0;
}