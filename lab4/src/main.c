#include <stdio.h>
#include <assert.h>
#include <malloc.h>
#include <ctype.h>
#include <stdlib.h>


enum TypeError { OK, DIV_BY_ZERO, SYN_ERR };

typedef struct {
	int arr[1001];
	int size;
}Stack;

int operation(int const first, int const second, int const op, int* error) {
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

void push(Stack* s_arr, int const input) {
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
			return SYN_ERR;
		}
		j--;

		if (count_num > count_zn + 1 || count_num < count_zn || count_brackets < 0) {
			return SYN_ERR;
		}
	}


	if (count_brackets > 0) {
		return SYN_ERR;
	}

	return OK;
}

int peek(Stack* s_arr) {
	return s_arr->arr[s_arr->size - 1];
}

int exe_and_pop(Stack* s_arr_num, Stack* s_arr_zn) {
	int error = 0;
	s_arr_num->arr[s_arr_num->size - 2] = operation(s_arr_num->arr[s_arr_num->size - 2], peek(s_arr_num), peek(s_arr_zn), &error);
	s_arr_num->size -= 1;
	s_arr_zn->size -= 1;
	return error;
}

int calc(const char input[], int const end, int* answer) {

	if (check_syn(input, end) == SYN_ERR) {
		return SYN_ERR;
	}

	Stack s_arr_num;
	s_arr_num.size = 0;
	Stack s_arr_zn;
	s_arr_zn.size = 0;
	int i = 0;
	int error = OK;

	while (i < end) {
		int cur = input[i];
		char* last_num = 0;

		if (cur == ' ') {}

		else if (isdigit(cur)) {
			int number = strtol(input + i, &last_num, 10);
			while (i < end && isdigit(input[i + 1])) {
				i = i + 1;
			}
			push(&s_arr_num, number);
		}

		else if (cur == '+' || cur == '-' || cur == '*' || cur == '/') {

			if (s_arr_zn.size == 0) {
				push(&s_arr_zn, cur);
			}

			else if (peek(&s_arr_zn) == '(') {
				push(&s_arr_zn, cur);
			}

			else if (cur == '*' || cur == '/') {
				if (peek(&s_arr_zn) != '+' && peek(&s_arr_zn) != '-') {
					error = exe_and_pop(&s_arr_num, &s_arr_zn);
				}
				push(&s_arr_zn, cur);

				if (error != OK) {
					return error;
				}

			}
			else {
				while (s_arr_zn.size > 0 && peek(&s_arr_zn) != '(' && error == OK) {
					error = exe_and_pop(&s_arr_num, &s_arr_zn);
				}

				if (error != OK) {
					return error;
				}

				push(&s_arr_zn, cur);
			}
		}

		else if (cur == '(') {
			push(&s_arr_zn, cur);
		}

		else if (s_arr_zn.size > 0 && i > 0 && cur == ')' && input[i - 1] != '(') {

			while (peek(&s_arr_zn) != '(' && error == OK) {
				error = exe_and_pop(&s_arr_num, &s_arr_zn);
			}

			if (error != OK) {
				return error;
			}
			s_arr_zn.size = s_arr_zn.size - 1;
		}
		i++;
	}

	while (s_arr_zn.size > 0 && error == OK) {
		error = exe_and_pop(&s_arr_num, &s_arr_zn);
	}

	if (error != OK) {
		return error;
	}

	*answer = s_arr_num.arr[0];
	return OK;
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
