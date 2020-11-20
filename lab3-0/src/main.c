#include <stdio.h>
#include <malloc.h>

void swap(int* x, int* y) {
	int z = *x;
	*x = *y;
	*y = z;
	return;
}

void merge_block(int text[], int first, int second_begin, int const end) {
	for (int i = first; i < second_begin && second_begin != end + 1; i++) {

		first += 1;
		int second_first = second_begin - first;
		if (text[i] > text[second_begin]) {
			swap(&text[i], &text[second_begin]);
			int count = 1;
			for (int j = first; j < first + (second_first) / 2; j++) {
				swap(&text[j], &text[second_begin - (count++)]);
			}

			count = 0;
			for (int j = first; j <= first + (second_first - 1) / 2; j++) {
				swap(&text[j], &text[second_begin - (count++)]);
			}

			second_begin += 1;
		}
	}

	return;
}

void merge_sort(int* array, int const begin, int const end) {
	int size_block = end - begin;

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

void print_answer(int const array[], int const size) {
	for (int i = 0; i < size; i++) {
		printf("%ld ", array[i]);
	}
	return;
}


int main() {
	int size = 0;
	if (scanf("%ld", &size) == 0) {
		return 0;
	}
	int* array;
	array = (int*)malloc(size * sizeof(int));
	if (array == NULL) {
		return 0;
	}

	for (int i = 0; i < size; i++) {
		if (scanf("%ld", &array[i]) == 0) {
			return 0;
		}
	}

	merge_sort(array, 0, size - 1);

	print_answer(array, size);
	free(array);
	return 0;
}
