#include <stdio.h>
#include <assert.h>
#include <malloc.h>
#include <ctype.h>


enum type_error { OK, DIV_BY_ZERO, SYN_ERR };

int operation(int first, int second, int op, int* error) {
	if (op == '*') {
		return first * second;
	}

	if (op == '/') {
		if (second != 0) {
			return first / second;
		}
		else {
			*error = DIV_BY_ZERO;
			return 0;
		}
	}

	if (op == '+') {
		return first + second;
	}

	if (op == '-') {
		return first - second;
	}

	*error = SYN_ERR;
	return 0;
}

void push(int* stack, int const input, int* arr_i) {
	*stack = input;
	*arr_i = *arr_i + 1;
	return;
}

void create_num(char input[], int* i, int* arr_num_i, int arr_num[], int const end) {
	int number = input[*i] - '0';
	while (*i < end && isdigit(input[*i + 1])) {
		number = number * 10 + input[*i + 1] - '0';
		*i = *i + 1;
	}
	push(&arr_num[*arr_num_i], number, arr_num_i);
	return;
}

int check_syn(char input[], int const end) {

	int count_brackets = 0;
	int count_num = 0;
	int count_zn = 0;
	int j = end - 1;
	while (j >= 0) {
		if (input[j] == ')') {
			j--;
			count_brackets = 1;
			break;
		}
		j--;
	}
	while (j >= 0 && count_brackets > 0) {
		if (input[j] == ' ') {
			j--;
			continue;
		}
		if (isdigit(input[j])) {
			j--;
			count_num++;

			while (j >= 0 && isdigit(input[j])) {
				j--;
			}
		}
		else if (input[j] == '+' || input[j] == '-' || input[j] == '*' || input[j] == '/') {
			count_zn++;
			j--;
		}
		else if (input[j] == ')') {
			count_brackets++;
			j--;
		}
		else if (input[j] == '(') {
			count_brackets--;
			j--;
		}

		if (count_num > count_zn + 1 || count_num < count_zn) {
			return SYN_ERR;
		}
	}


	if (count_brackets > 0) {
		return SYN_ERR;
	}

	return OK;
}

int exe_and_pop(int arr_num[], int* arr_num_i, int arr_zn[], int* arr_zn_i) {
	int error = 0;
	arr_num[*arr_num_i - 2] = operation(arr_num[*arr_num_i - 2], arr_num[*arr_num_i - 1], arr_zn[*arr_zn_i - 1], &error);
	*arr_num_i -= 1;
	*arr_zn_i -= 1;
	return error;
}

int calc(char input[], int end, int* answer) {

	if (check_syn(input, end) == SYN_ERR) {
		return SYN_ERR;
	}

	int arr_num[502];
	int arr_zn[1001];
	int i = 0;
	int arr_num_i = 0;
	int arr_zn_i = 0;
	int error = OK;

	while (i < end) {
		int cur = input[i];

		if (cur == ' ') {}

		else if (isdigit(cur)) {
			create_num(input, &i, &arr_num_i, arr_num, end);
		}

		else if (cur == '+' || cur == '-' || cur == '*' || cur == '/') {

			if (arr_zn_i == 0) {
				if (arr_num_i == 0 || arr_num_i >= 2) {
					return SYN_ERR;
				}

				push(&arr_zn[0], cur, &arr_zn_i);
			}

			else if (arr_zn[arr_zn_i - 1] == '(') {
				push(&arr_zn[arr_zn_i], cur, &arr_zn_i);
			}

			else if (arr_num_i <= 1) {
				return SYN_ERR;
			}

			else if (cur == '*' || cur == '/') {
				if (arr_zn[arr_zn_i - 1] == '+' || arr_zn[arr_zn_i - 1] == '-') {
					push(&arr_zn[arr_zn_i], cur, &arr_zn_i);
				}
				else {
					error = exe_and_pop(arr_num, &arr_num_i, arr_zn, &arr_zn_i);
					push(&arr_zn[arr_zn_i], cur, &arr_zn_i);
				}

				if (error != OK) {
					return error;
				}

			}
			else {
				while (arr_zn_i > 0 && arr_zn[arr_zn_i - 1] != '(' && error == OK) {
					error = exe_and_pop(arr_num, &arr_num_i, arr_zn, &arr_zn_i);
				}

				if (error != OK) {
					return error;
				}

				push(&arr_zn[arr_zn_i], cur, &arr_zn_i);
			}
		}

		else if (cur == '(') {
			push(&arr_zn[arr_zn_i], cur, &arr_zn_i);
		}

		else if (arr_zn_i > 0 && i > 0 && cur == ')' && input[i - 1] != '(') {

			while (arr_zn[arr_zn_i - 1] != '(' && error == OK) {
				if (arr_num_i > 1 && arr_zn_i > 1) {
					error = exe_and_pop(arr_num, &arr_num_i, arr_zn, &arr_zn_i);
				}
				else {
					return SYN_ERR;
				}
			}

			if (error != OK) {
				return error;
			}

			arr_zn_i -= 1;
		}

		else {
			return SYN_ERR;
		}

		i++;
	}

	if (arr_num_i == arr_zn_i + 1) {
		while (arr_zn_i > 0 && error == OK) {
			error = exe_and_pop(arr_num, &arr_num_i, arr_zn, &arr_zn_i);
		}
	}
	else {
		return SYN_ERR;
	}

	if (error != OK) {
		return error;
	}

	*answer = arr_num[0];
	return 0;
}


int main() {

	char input[1001];
	int length = 0;

	for (int i = 0; i < 1001; i++) {
		if (scanf("%c", &input[i]) != 1) {
			break;
		}
		if (input[i] == '\n') {
			break;
		}

		length += 1;
	}

	if (length == 0) {
		printf("syntax error");
		return 0;
	}


	int answer = 0;
	int status_code = calc(input, length, &answer);

	if (status_code == OK) {
		printf("%d", answer);
	}
	else if (status_code == DIV_BY_ZERO) {
		printf("division by zero");
	}
	else if (status_code == SYN_ERR) {
		printf("syntax error");
	}

	return 0;
}
