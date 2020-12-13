#include <stdio.h>
#include <assert.h>
#include <malloc.h>

int operation(int first, int second, char op, int* error) {
	if (op == '*') {
		return first * second;
	}
	else {
		if (op == '/') {
			if (second != 0) {
				return first / second;
			}
			else {
				*error = 1;
				return 0;
			}
		}
		else {
			if (op == '+') {
				return first + second;
			}
			else {
				return first - second;
			}
		}
	}
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
		if (input[i] != ' ') {
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
			}
			else {
				if (input[i] == '+' || input[i] == '-' || input[i] == '*' || input[i] == '/') {

					if (arr_zn_i == 0) {
						arr_zn[0] = input[i];
						arr_zn_i += 1;
					}
					else {
						if (arr_zn[arr_zn_i - 1] == '(') {
							arr_zn[arr_zn_i] = input[i];
							arr_zn_i += 1;
						}
						else {
							if (arr_num_i > 1) {
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
								}
								else {
									while (arr_zn_i > 0  && arr_zn[arr_zn_i - 1] != '(') {
										arr_num[arr_num_i - 2] = operation(arr_num[arr_num_i - 2], arr_num[arr_num_i - 1], arr_zn[arr_zn_i - 1], &error);
										arr_num_i -= 1;
										arr_zn_i -= 1;
									}

									arr_zn[arr_zn_i] = input[i];
									arr_zn_i += 1;
								}
							}
							else {
								return 2;
							}
						}
					}

				}
				else {
					if (input[i] == '(') {
						arr_zn[arr_zn_i] = input[i];
						arr_zn_i += 1;
					}
					else {
						if (arr_zn_i >0 && i > 0 && input[i] == ')' && input[i - 1] != '(') {
							while (arr_zn[arr_zn_i - 1] != '(') {
								if (arr_num_i > 1 && arr_zn_i > 0) {
									arr_num[arr_num_i - 2] = operation(arr_num[arr_num_i - 2], arr_num[arr_num_i - 1], arr_zn[arr_zn_i - 1], &error);
									arr_num_i -= 1;
									arr_zn_i -= 1;
								}
								else {
									return 2;
								}
							}

							arr_zn_i -= 1;
						}
						else {
							return 2;
						}
					}
				}
			}

		}

		i++;
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

	int is_correct = calc(input, length, arr_num, arr_zn);

	if (is_correct == 0) {
		printf("%d", arr_num[0]);
	}
	else {
		if (is_correct == 1) {
			printf("division by zero");
		}
		else {
			printf("syntax error");
		}
	}

	return 0;
}
