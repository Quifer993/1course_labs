#include <stdio.h>
#include <math.h>


int print_result(int number, int length_template, unsigned char* template, unsigned char* text) {
	for (int i = 0; i < length_template; i++) {
		printf("%d ", number - length_template + i + 1);
		if (template[i] != text[(number + i )%length_template]){
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
		ht += ( template[i] % 3 ) * power;
		power *= 3;
		length_template += 1;
	}
	printf("%d ", ht);

	unsigned char text[17];
	int hs = 0; //hash string
	power = 1;
	int number = length_template;
	for (int i = 0; i < length_template; i++) {
		if (fscanf(in, "%c", &text[i]) == EOF) {
			fclose(in);
			return 0;
		}
		hs += (text[i] % 3) * power;
		power *= 3;
	}
	power /= 3;


	if (hs == ht) {
		print_result(number, length_template, template, text);
	}

	while (fscanf(in, "%c", &text[number % length_template]) != EOF) {
		hs = hs / 3 + power * (text[number % length_template] % 3 );
		number += 1;
		if (hs == ht) {
			print_result(number, length_template, template, text);
		}
	}

	fclose(in);
	return 0;
}
