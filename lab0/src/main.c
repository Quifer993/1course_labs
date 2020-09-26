#include <stdio.h>
#include <math.h>
//#pragma warning(disable : 4996)


int main() {
    // ввод оснований
    int b1, b2;

    if (!(scanf("%d %d", &b1, &b2))) {
        return 0;
    }

    //проверка оснований
    if (b1 < 2 || b1>16 || b2 < 2 || b2>16) {
        printf("bad input");
        return 0;
    }
    
    char number[14];

    if (!(scanf("%13s", number))) {
        return 0;
    }

    int point = 100, end_num = 13;
    long double num = 0;
    //поиск точки

    for (int i = 0; i < 14; i++) {
        if ((number[i] == '.') || ((number[i] >= '0') && (number[i] <= '9')) || \
            ((number[i] >= 'A') && (number[i] <= 'F')) || ((number[i] >= 'a') && (number[i] <= 'f')) || (number[i] == '\0')) {
            if (number[i] == '.') {
                if (point == 100) {
                    point = i;
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

    for (int i = end_num - 1; i >= 0; i--) {
        //a -> A, b -> B,..
        if ((number[i] - 'a') >= 0) {
            number[i] = (number[i] + 'A' - 'a');
        }
        //перевод из b1 в 10чную
        if (!(i == point)) {
            if (i < point) {
                if ((number[i] <= '9')) {
                    if (number[i] < b1 + '0') {
                        num = num + (number[i] - '0') * pow(b1, point - i - 1);
                    }
                    else {
                        printf("bad input");
                        return 0;
                    }
                }
                else {
                    if (number[i] >= 'A') {
                        if (number[i] < (b1 + '7')) {
                            num = num + (number[i] - '7') * pow(b1, point - i - 1);
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
                    if (number[i] < b1 + '0') {
                        num = num + (number[i] - '0') * pow(b1, point - i);
                    }
                    else {
                        printf("bad input");
                        return 0;
                    }
                }
                else {
                    if (number[i] >= 'A') {
                        if (number[i] < (b1 + '7')) {
                            num = num + (number[i] - '7') * pow(b1, point - i);
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

    int length_of_second = 0, end;
    char answer[62];

    //Нахождение длины числа в b2 с.с.
    for (int i = 1; i < 49; i++) {
        if (pow(b2, i) > num) {
            length_of_second = i;
            break;
        }
    }

    int long long answer10int = num / 1;
    double answer10_after_point = num - answer10int;
    end = length_of_second;

    for (int i = length_of_second - 1; i >= 0; i--) {
        if (answer10int % b2 < 10) {
            answer[i] = (answer10int % b2 + '0');
        }
        else {
            answer[i] = (answer10int % b2 + '7');
        }
        answer10int = answer10int / b2;
    }

    if (!(num == (int)num)) {
        answer[length_of_second] = '.';
        for (int i = length_of_second + 1; i < length_of_second + 14; i++) {
            if (answer10_after_point == 0) {
                end = i;
                break;
            }
            answer10_after_point = answer10_after_point * b2;
            if ((int)answer10_after_point < 10) {
                answer[i] = ((int)answer10_after_point + '0');
            }
            else {
                answer[i] = ((int)answer10_after_point + '7');
            }
            end = i;
            answer10_after_point = answer10_after_point - (int)answer10_after_point;
        }
    }

    answer[end] = '\0';
    printf("%s\n", answer);
    return 0;
}
