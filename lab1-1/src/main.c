#include <stdio.h>
#include <math.h>

unsigned int make_hash(unsigned char text[], long long int *power, int length) {
	unsigned int hash = 0;

	for (int i = 0; i < length; i++) {
		hash += (text[i] % 3) * *power;
		*power *= 3;
	}
	*power /= 3;
	return hash;
}

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
	const unsigned int ht = make_hash(template, &power, length_template);//hash template
	printf("%d ", ht);

	unsigned char text[17]; 
	int number = length_template;
	for (int i = 0; i < length_template; i++) {
		if (fscanf(in, "%c", &text[i]) == EOF) {
			fclose(in);
			return 0;
		}
	}
	
	power = 1;
	unsigned int hs = make_hash(text, &power, length_template); //hash string

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
