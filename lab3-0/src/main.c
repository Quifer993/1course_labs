#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#pragma warning (disable:4996)

void swap(int* x, int* y) {
	int z = *x;
	*x = *y;
	*y = z;
	return;
}

void quick_sort(int* array, int const begin, int const end) {
	if (end - begin < 1) {
		return;
	}
	int left = begin;
	int right = end;
	int	pivot = (begin + end) / 2;

	while (left != right) {

		while (left != pivot) {
			if (array[left] > array[pivot]) {
				break;
			}
			left += 1;
		}

		while (right != pivot) {
			if (array[right] < array[pivot]) {
				break;
			}
			right -= 1;
		}

		if (left == right) {
			break;
		}
		else {
			if (left == pivot) {
				swap(&array[pivot], &array[right]);
				pivot = right;
				continue;
			}
			else {
				if (right == pivot) {
					swap(&array[pivot], &array[left]);
					pivot = left;
					continue;
				}
			}
			swap(&array[left], &array[right]);
		}
	}
	quick_sort(array, begin, pivot - 1);
	quick_sort(array, pivot + 1, end);
	return;
}

void print_answer(int const array[], int const size) {
	for (int i = 0; i < size; i++) {
		printf("%d ", array[i]);
	}
	return;
}


int main() {
	int size = 0;
	if (scanf("%d", &size) == 0) {
		return 0;
	}
	int* array;
	array = (int*)malloc(size * sizeof(int));
	if (array == NULL) {
		return 0;
	}

	for (int i = 0; i < size; i++) {
		if (scanf("%d", &array[i]) == 0) {
			return 0;
		}
	}

	quick_sort(array, 0, size - 1);

	print_answer(array, size);
	free(array);
	return 0;
}
