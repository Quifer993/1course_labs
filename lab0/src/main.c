#include <stdio.h>
#include <math.h>
#pragma warning(disable : 4996)
int main() {
    // ввод оснований
    int first, second;
    if (!(scanf("%d %d", &first, &second)))
        return 0;
    //проверка оснований
    if (first < 2 || first>16 || second < 2 || second>16) {
        printf("bad input");
        return 0;
    }
    //ввод числа
    char number[15];
    if (!(scanf("%s", &number)))
        return 0;
    int point = 100, point_for_check = 100, end_num, i;
    long double num = 0;
    //поиск точки
    for (i = 0; i < 14; i++) {
        if ((number[i] == '.') || ((number[i] >= '0') && (number[i] <= '9')) || \
            ((number[i] >= 'A') && (number[i] <= 'F')) || ((number[i] >= 'a') && (number[i] <= 'f')) || (number[i] == '\0')) {
            if (number[i] == '.') {
                if (point == 100) {
                    point = i;
                    point_for_check = i;;
                }
                else {
                    printf("bad input");
                    return 0;
                }
            }
            if (number[i] == '\0') {
                end_num = i;
                break;
            }
        }
        else {
            printf("bad input");
            return 0;
        }
    }
    if ((end_num - point == 1) || (point == 0)) {
        printf("bad input");
        return 0;
    }
    if (point == 100) {
        point = end_num;
    }
    for (i = end_num - 1; i >= 0; i--) {
        //a -> A, b -> B,..
        if ((number[i] - 'a') >= 0) {
            number[i] = (number[i] + 'A' - 'a');
        }
        //перевод из b1 в 10чную
        if (!(i == point)) {
            if (i < point) {
                if ((number[i] <= '9')) {
                    if (number[i] < first + '0') {
                        num = num + (number[i] - '0') * pow(first, point - i - 1);
                    }
                    else {
                        printf("bad input");
                        return 0;
                    }
                }
                else {
                    if (number[i] >= 'A') {
                        if (number[i] < (first + '7')) {
                            num = num + (number[i] - '7') * pow(first, point - i - 1);
                        }
                        else {
                            printf("bad input");
                            return 0;
                        }
                    }
                }
            }
            else {
                if ((number[i] <= '9')) {
                    if (number[i] < first + '0') {
                        num = num + (number[i] - '0') * pow(first, point - i);
                    }
                    else {
                        printf("bad input");
                        return 0;
                    }
                }
                else {
                    if (number[i] >= 'A') {
                        if (number[i] < (first + '7')) {
                            num = num + (number[i] - '7') * pow(first, point - i);
                        }
                        else {
                            printf("bad input");
                            return 0;
                        }
                    }
                }
            }

        }

    }
    int length_of_second, end;

    //Нахождение длины числа в 10чной и b2
    char answer[62];

    for (i = 1; i < 50; i++) {
        if (pow(second, i) > num) {
            length_of_second = i;
            break;
        }
    }
    if (num == (int)num) {
        point_for_check = 100;
    }
    int long long answer10 = num / 1;
    int answer_remainder;
    end = length_of_second;
    long double answer10_after_point = num - answer10, answer10_after_point_int;
    for (i = length_of_second - 1; i >= 0; i--) {
        answer_remainder = answer10 % second;
        if (answer_remainder < 10) {
            answer[i] = (answer_remainder + '0');
        }
        else {
            answer[i] = (answer_remainder + '7');
        }
        answer10 = answer10 / second;
    }
    //Вывод только целых
    if (point_for_check == 100) {
        answer[end] = '\0';
        printf("%s\n", answer);
        return 0;
    }
    //Вывод дробных
    else {
        answer[length_of_second] = '.';
        for (i = length_of_second + 1; i < length_of_second + 14; i++) {
            if (answer10_after_point == 0) {
                end = i;
                break;
            }
            answer10_after_point = answer10_after_point * second;
            answer10_after_point_int = (int)answer10_after_point;
            if (answer10_after_point_int < 10) {
                answer[i] = (answer10_after_point_int + '0');
            }
            else {
                answer[i] = (answer10_after_point_int + '7');
            }
            end = i;
            answer10_after_point = answer10_after_point - answer10_after_point_int;
        }
    }
    answer[end] = '\0';
    printf("%s\n", answer);
    return 0;
}
