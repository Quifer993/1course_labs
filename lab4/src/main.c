#include <stdio.h>
#include <assert.h>
#include <malloc.h>


int operation(int first, int second, char op, int* error) {
	if (op == '*') {
		return first * second;
	}

	if (op == '/') {
		if (second != 0) {
			return first / second;
		}
		else {
			*error = 1;
			return 0;
		}
	}

	if (op == '+') {
		return first + second;
	}

	if (op == '-') {
		return first - second;
	}

	return 0;
}

int calc(char input[], int end, int arr_num[], char arr_zn[]) {
	int i = 0;
	int arr_num_i = 0;
	int arr_zn_i = 0;
	int error = 0;
	int negative = 0;

	if (input[0] == '-') {
		negative = 1;
		i++;
	}

	while (i < end) {
		if (input[i] == ' ') {
			i++;
			continue;
		}

		if (input[i] <= '9' && input[i] >= '0') {
			int number = input[i] - '0';
			while (input[i + 1] <= '9' && input[i + 1] >= '0') {
				number = number * 10 + input[i + 1] - '0';
				i++;
			}

			if (negative == 1) {
				number = 0 - number;
				negative = 0;
			}

			arr_num[arr_num_i] = number;
			arr_num_i += 1;

			i++;
			continue;
		}

		if (input[i] == '+' || input[i] == '-' || input[i] == '*' || input[i] == '/') {

			if (arr_zn_i == 0) {
				arr_zn[0] = input[i];
				arr_zn_i += 1;
				i++;
				continue;
			}

			if (arr_zn[arr_zn_i - 1] == '(') {
				arr_zn[arr_zn_i] = input[i];
				arr_zn_i += 1;
				i++;
				continue;
			}

			if (arr_num_i <= 1) {
				return 2;
			}

			if (input[i] == '*' || input[i] == '/') {
				if (arr_zn[arr_zn_i - 1] == '+' || arr_zn[arr_zn_i - 1] == '-') {
					arr_zn[arr_zn_i] = input[i];
					arr_zn_i += 1;
				}
				else {
					arr_num[arr_num_i - 2] = operation(arr_num[arr_num_i - 2], arr_num[arr_num_i - 1], arr_zn[arr_zn_i - 1], &error);
					arr_num_i -= 1;
					arr_zn[arr_zn_i - 1] = input[i];
				}

				if (error == 1) {
					return 1;
				}

				i++;
				continue;
			}

			while (arr_zn_i > 0 && arr_zn[arr_zn_i - 1] != '(') {
				arr_num[arr_num_i - 2] = operation(arr_num[arr_num_i - 2], arr_num[arr_num_i - 1], arr_zn[arr_zn_i - 1], &error);
				arr_num_i -= 1;
				arr_zn_i -= 1;
			}

			if (error == 1) {
				return 1;
			}

			arr_zn[arr_zn_i] = input[i];
			arr_zn_i += 1;
			i++;
			continue;

		}

		if (input[i] == '(') {
			arr_zn[arr_zn_i] = input[i];
			arr_zn_i += 1;
			i++;
			continue;
		}

		if (arr_zn_i > 0 && i > 0 && input[i] == ')' && input[i - 1] != '(') {
			int correct_brackets = 0;
			for (int j = i - 1; j >= 0 && input[j] != '('; j--) {
				if (input[j] <= '9' && input[j] >= '0') {
					correct_brackets = 1;
					break;
				}
			}

			while (arr_zn[arr_zn_i - 1] != '(') {
				if (arr_num_i > 1 && arr_zn_i > 1 && correct_brackets == 1) {
					arr_num[arr_num_i - 2] = operation(arr_num[arr_num_i - 2], arr_num[arr_num_i - 1], arr_zn[arr_zn_i - 1], &error);
					arr_num_i -= 1;
					arr_zn_i -= 1;
				}
				else {
					return 2;
				}
			}

			if (error == 1) {
				return 1;
			}

			arr_zn_i -= 1;
			i++;
			continue;
		}

		return 2;
	}

	if (arr_num_i == arr_zn_i + 1) {
		while (arr_zn_i > 0) {
			arr_num[arr_num_i - 2] = operation(arr_num[arr_num_i - 2], arr_num[arr_num_i - 1], arr_zn[arr_zn_i - 1], &error);
			arr_num_i -= 1;
			arr_zn_i -= 1;
		}
	}
	else {
		return 2;
	}

	if (error == 1) {
		return 1;
	}

	return 0;
}


int main() {

	char input[1001];
	int length = 0;

	for (int i = 0; i < 1001; i++) {
		if (scanf("%c", &input[i]) == 0) {
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


	int arr_num[1001];
	char arr_zn[1001];

	int status_code = calc(input, length, arr_num, arr_zn);

	if (status_code == 0) {
		printf("%d", arr_num[0]);
	}
	else {
		if (status_code == 1) {
			printf("division by zero");
		}
		else {
			printf("syntax error");
		}
	}

	return 0;
}
