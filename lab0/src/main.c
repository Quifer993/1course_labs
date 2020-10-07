#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int is_input_right(int b1, int b2, short int input) {
    if (input != 2 || b1 < 2 || b1>16 || b2 < 2 || b2>16) {
        return 1;
    }
    return 0;
}

int for_A_to_a(char number) {
    if (number >= 'A' && number <= 'F') {
        number = (number - 'A' + 'a');
    }
    return number;
}

int check_bad_input(char number[], int b1) {
    for (int i = 0; number[i] != '\0'; i++) {
        if (number[i] != '.' && (number[i] < '0' || number[i]>'9') && (number[i] < 'a' || number[i]>'f')) {
            return 1;
        }
        if (number[i] <= '9') {
            if (number[i] >= b1 + '0') {
                return 1;
            }
        }
        else {
            if (number[i] >= b1 + 'a' - 10) {
                return 1;
            }
        }
    }
    return 0;
}

int checkup_point(char number[], int* point, int* end_num) {
    for (int i = 0; number[i] != '\0'; i++) {
        if (number[i] == '.') {
            if (*point == -1) {
                *point = i;
            }
            else {
                return 1;
            }

        }
        *end_num = i + 1;
    }
    if ((*end_num - *point == 1) || (*point == 0)) {
        return 1;
    }

    if (*point == -1) {
        *point = *end_num;
    }
    return 0;
}

double translation(char number, int b1, int power) {
    if ((number <= '9')) {
        return (number - '0') * pow(b1, power);
    }
    return (number + 10 - 'a') * pow(b1, power);
}


int main() {
    int b1, b2;
    short int input = (scanf("%d %d", &b1, &b2));
    int error1 = is_input_right(b1, b2, input);

    int point = -1;
    int end_num = 0;
    char number[14];

    if (scanf("%13s", &number) != 1) {
        printf("bad input");
        return 0;
    }

    for (int i = 0; number[i] != '\0'; i++) {
        number[i] = for_A_to_a(number[i]);
    }

    int error2 = check_bad_input(number, b1);
    int error3 = checkup_point(number, &point, &end_num);
    if (error1 == 1 || error2 == 1 || error3 == 1) {
        printf("bad input");
        return 0;
    }

    long long unsigned num_integer = 0;
    double fractional = 0;
    for (int i = end_num - 1; i >= 0; i--) {
        if (i < point) {
            int power_i = point - i - 1;
            num_integer = num_integer + translation(number[i], b1, power_i);
        }
        if (i > point) {
            int power_f = point - i;
            fractional = fractional + translation(number[i], b1, power_f);
        }
    }

    int length_of_second = 0;
    const int MAX_INT = 49;

    for (int i = 1; i < MAX_INT; i++) {
        if (pow(b2, i) > num_integer) {
            length_of_second = i;
            break;
        }
    }

    char answer[62];

    for (int i = length_of_second - 1; i >= 0; i--) {
        if (num_integer % b2 < 10) {
            answer[i] = num_integer % b2 + '0';
        }
        else {
            answer[i] = num_integer % b2 + 'a' - 10;
        }
        num_integer = num_integer / b2;
    }

    int end = length_of_second;
    if (fractional != 0) {
        answer[length_of_second] = '.';
        for (int i = length_of_second + 1; i < length_of_second + 14; i++) {
            if (fractional == 0) {
                end = i;
                break;
            }
            fractional = fractional * b2;
            int int_fractional = fractional;
            if (int_fractional < 10) {
                answer[i] = int_fractional + '0';
            }
            else {
                answer[i] = int_fractional + 'a' - 10;
            }
            end = i;
            fractional = fractional - int_fractional;
        }
    }

    answer[end] = '\0';
    printf("%s\n", answer);
    return 0;
}
