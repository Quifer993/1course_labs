#include <stdio.h>
#include <assert.h>
#include <malloc.h>
#include <ctype.h>
#include <stdlib.h>


enum TypeError { Ok, DivByZero, SynError };

typedef struct {
	int arr[1001];
	int size;
}STACK;

int operation(int const first, int const second, int const op, int* error) {
	if (op == '*') {
		return first * second;
	}

	if (op == '/') {
		if (second != 0) {
			return first / second;
		}
		else {
			*error = DivByZero;
			return 0;
		}
	}

	if (op == '+') {
		return first + second;
	}

	if (op == '-') {
		return first - second;
	}

	*error = SynError;
	return 0;
}

void push(STACK* s_arr, int const input) {
	s_arr->arr[s_arr->size] = input;
	s_arr->size++;
}

int check_syn(const char input[], int const end) {

	int count_brackets = 0;
	int count_num = 0;
	int count_zn = 0;
	int j = end - 1;

	while (j >= 0) {

		if (input[j] == ')') {
			count_brackets += 1;
		}

		else if (input[j] == '(' && input[j + 1] != ')') {
			count_brackets--;
		}

		else if (input[j] == ' ') {}

		else if (isdigit(input[j])) {
			count_num++;

			while (j > 0 && isdigit(input[j - 1])) {
				j--;
			}
		}
		else if (input[j] == '+' || input[j] == '-' || input[j] == '*' || input[j] == '/') {
			count_zn++;
		}
		else {
			return SynError;
		}
		j--;

		if (count_num > count_zn + 1 || count_num < count_zn || count_brackets < 0) {
			return SynError;
		}
	}


	if (count_brackets > 0) {
		return SynError;
	}

	return Ok;
}

int exe_and_pop(STACK* s_arr_num, STACK* s_arr_zn) {
	int error = 0;
	s_arr_num->arr[s_arr_num->size - 2] = operation(s_arr_num->arr[s_arr_num->size - 2], s_arr_num->arr[s_arr_num->size - 1], s_arr_zn->arr[s_arr_zn->size - 1], &error);
	s_arr_num->size -= 1;
	s_arr_zn->size -= 1;
	return error;
}

int calc(const char input[], int const end, int* answer) {

	if (check_syn(input, end) == SynError) {
		return SynError;
	}

	STACK s_arr_num;
	s_arr_num.size = 0;
	STACK s_arr_zn;
	s_arr_zn.size = 0;
	int i = 0;
	int error = Ok;
	int last_num = 0;

	while (i < end) {
		int cur = input[i];

		if (cur == ' ') {}

		else if (isdigit(cur)) {
			int number = strtol(input + i, last_num, 10);
			while (i < end && isdigit(input[i + 1])) {
				i = i + 1;
			}
			push(&s_arr_num, number);
		}

		else if (cur == '+' || cur == '-' || cur == '*' || cur == '/') {

			if (s_arr_zn.size == 0) {
				push(&s_arr_zn, cur);
			}

			else if (s_arr_zn.arr[s_arr_zn.size - 1] == '(') {
				push(&s_arr_zn, cur);
			}

			else if (cur == '*' || cur == '/') {
				if (s_arr_zn.arr[s_arr_zn.size - 1] == '+' || s_arr_zn.arr[s_arr_zn.size - 1] == '-') {}
				else {
					error = exe_and_pop(&s_arr_num, &s_arr_zn);
				}
				push(&s_arr_zn, cur);

				if (error != Ok) {
					return error;
				}

			}
			else {
				while (s_arr_zn.size > 0 && s_arr_zn.arr[s_arr_zn.size - 1] != '(' && error == Ok) {
					error = exe_and_pop(&s_arr_num, &s_arr_zn);
				}

				if (error != Ok) {
					return error;
				}

				push(&s_arr_zn, cur);
			}
		}

		else if (cur == '(') {
			push(&s_arr_zn, cur);
		}

		else if (s_arr_zn.size > 0 && i > 0 && cur == ')' && input[i - 1] != '(') {

			while (s_arr_zn.arr[s_arr_zn.size - 1] != '(' && error == Ok) {
				error = exe_and_pop(&s_arr_num, &s_arr_zn);
			}

			if (error != Ok) {
				return error;
			}
			s_arr_zn.size = s_arr_zn.size - 1;
		}
		i++;
	}

	while (s_arr_zn.size > 0 && error == Ok) {
		error = exe_and_pop(&s_arr_num, &s_arr_zn);
	}

	if (error != Ok) {
		return error;
	}

	*answer = s_arr_num.arr[0];
	return Ok;
}


int main() {

	char input[1002];
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
	input[length] = '\n';

	int answer = 0;
	int status_code = calc(input, length, &answer);

	if (status_code == Ok) {
		printf("%d", answer);
	}
	else if (status_code == DivByZero) {
		printf("division by zero");
	}
	else if (status_code == SynError) {
		printf("syntax error");
	}

	return 0;
}
