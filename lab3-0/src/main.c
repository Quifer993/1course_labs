#include <stdio.h>
#include <malloc.h>

void swap(long int* x, long int* y) {
	long int z = *x;
	*x = *y;
	*y = z;
	return;
}

void merge_block(long int text[], long int first, long int second_begin, long int const end) {
	for (long int i = first; i < second_begin && second_begin != end + 1; i++) {

		first += 1;
		long int second_first = second_begin - first;
		if (text[i] > text[second_begin]) {
			swap(&text[i], &text[second_begin]);
			long int count = 1;
			for (long int j = first; j < first + (second_first) / 2; j++) {
				swap(&text[j], &text[second_begin - (count++)]);
			}

			count = 0;
			for (long int j = first; j <= first + (second_first - 1) / 2; j++) {
				swap(&text[j], &text[second_begin - (count++)]);
			}

			second_begin += 1;
		}
	}

	return;
}

void merge_sort(long int* array, long int const begin, long int const end) {
	long int size_block = end - begin;

	if (size_block < 1) {
		return;
	}
	else {
		if (size_block > 1) {
			merge_sort(array, begin, begin + (end - begin - 1) / 2);
			merge_sort(array, begin + (end - begin + 1) / 2, end);
		}
	}

	merge_block(array, begin, begin + (end - begin + 1) / 2, end);

	return;
}

void print_answer(long int const array[], long int const size) {
	for (long int i = 0; i < size; i++) {
		printf("%ld ", array[i]);
	}
	return;
}


int main() {
	long int size = 0;
	if (scanf("%ld", &size) == 0) {
		return 0;
	}
	long int* array;
	array = (long int*)malloc(size * sizeof(long));
	if (array == NULL) {
		return 0;
	}

	for (long int i = 0; i < size; i++) {

		if (scanf("%ld", &array[i]) == 0) {
			return 0;
		}

	}


	merge_sort(array, 0, size - 1);

	print_answer(array, size);
	free(array);
	return 0;
}
