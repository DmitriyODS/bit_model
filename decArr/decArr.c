#include "decArr.h"

const char *enter_txt = "Enter your number in style: +/-123.45, max len number = 24 symbol without symbol +/- and dot. q - for stop enter->\n";

byte_t *getDecArrFromStdin() {
    byte_t *block = (byte_t *) calloc(ARR_SIZE, sizeof(byte_t));

    printf("%s", enter_txt);

    int buf = 0;
    int buf_order = 0;
    int order = 0;
    byte_t count_missed = 0;
    byte_t pointer = P_M;
    bool is_dot = false;

    for (byte_t i = 0; i < M_SIZE; ++i) {
        buf = getchar();

        if (buf == EOF || buf == 'q' || buf == 'Q') {
            break;
        } else if (buf == '+') {
            block[P_ZN] = 0;
            --i;
        } else if (buf == '-') {
            block[P_ZN] = 9;
            --i;
        } else if (buf == '.') {
            is_dot = true;
            --i;
        } else if (buf == '0') {
            if (is_dot) {
                if (buf_order > -1) {
                    ++buf_order;
                } else {
                    block[pointer++] = 0;
                }
            } else if (pointer > P_M) {
                block[pointer++] = 0;
                ++order;
            }
        } else if (buf > '0' && buf <= '9') {
            if (is_dot) {
                if (buf_order > -1) {
                    order -= buf_order;
                    buf_order = -1;
                }
            } else {
                buf_order = -1;
                ++order;
            }

            block[pointer++] = buf - '0';
        } else {
            ++count_missed;
        }
    }

    printf("Count missed invalid symbol = %d\n", count_missed);

    writeSignedNumToDecArr(block + P_ORDER, block + O_SIZE, order);

    return block;
}

void writeSignedNumToDecArr(const byte_t *s_ptr, byte_t *e_ptr, int num) {
    if (num < 0) {
        for (byte_t *ptr = e_ptr; ptr >= s_ptr; --ptr) {
            *ptr = num % 10 + 9;
            num /= 10;
        }

        ++(*e_ptr);
    } else {
        for (byte_t *ptr = e_ptr; ptr >= s_ptr; --ptr) {
            *ptr = num % 10;
            num /= 10;
        }
    }
}

int scanDecArrToSignedNum(const byte_t *s_ptr, const byte_t *e_ptr) {
    int res = 0;
    int cur_pos = 1;

    if (s_ptr[0] == 9) {
        for (int i = 0; e_ptr - i > s_ptr; ++i) {
            res += (9 - *(e_ptr - i)) * cur_pos;
            cur_pos *= 10;
        }

        ++res;
        res *= -1;
    } else {
        for (int i = 0; e_ptr - i > s_ptr; ++i) {
            res += *(e_ptr - i) * cur_pos;
            cur_pos *= 10;
        }
    }

    return res;
}

void printDecArr(byte_t *s_ptr, const byte_t *e_ptr) {
    for (byte_t *ptr = s_ptr; ptr <= e_ptr; ++ptr) {
        printf("%d", *ptr);
    }
}

const char *getNumFromDecArr(const byte_t *s_ptr) {
    // выделяем прям по полной, не хочу считать что - то
    char *res = (char *) calloc(ARR_SIZE + 1, sizeof(char));

    int order = scanDecArrToSignedNum(s_ptr + P_ORDER, s_ptr + P_M - 1);
    int e_ptr_num = 0;
    int s_write_ptr = 0;

    if (order == 0 && s_ptr[P_M] == 0) {
        res[s_write_ptr++] = '0';
        res[s_write_ptr++] = '.';
        res[s_write_ptr++] = '0';

        return res;
    }

    if (s_ptr[P_ZN] == 9) {
        res[s_write_ptr++] = '-';
    }

    for (int i = ARR_SIZE - 1; i >= P_M; --i) {
        if (s_ptr[i] != 0 || P_M + order - 1 == i) {
            e_ptr_num = i;
            break;
        }
    }

    if (order < 1) {
        res[s_write_ptr++] = '0';
        res[s_write_ptr++] = '.';

        for (int i = 0; i < -order; ++i) {
            res[s_write_ptr++] = '0';
        }

        for (int i = P_M; i <= e_ptr_num; ++i) {
            res[s_write_ptr++] = (char) (s_ptr[i] + '0');
        }
    } else {
        for (int i = P_M; i < P_M + order; ++i) {
            res[s_write_ptr++] = (char) (s_ptr[i] + '0');
        }

        res[s_write_ptr++] = '.';
        res[s_write_ptr] = '0';

        for (int i = P_M + order; i <= e_ptr_num; ++i) {
            res[s_write_ptr++] = (char) (s_ptr[i] + '0');
        }
    }

    return res;
}

void shiftDecArr(byte_t *s_ptr, byte_t *e_ptr, int shift) {
    if (shift < 0) {
        for (byte_t *p = e_ptr; p >= s_ptr; --p) {
            if (p < s_ptr - shift) {
                *p = 0;
            } else {
                *p = *(p + shift);
            }
        }
    } else {
        for (byte_t *p = s_ptr; p <= e_ptr; ++p) {
            if (p > e_ptr - shift) {
                *p = 0;
            } else {
                *p = *(p + shift);
            }
        }
    }
}

void decArrAddNum(const byte_t *s_ptr, byte_t *e_ptr, unsigned int num) {
    unsigned int buf = num;

    for (byte_t *p = e_ptr; p >= s_ptr && buf > 0; --p) {
        buf += *p;
        *p = buf % 10;
        buf /= 10;
    }
}

void negDecArr(byte_t *s_ptr, byte_t *e_ptr) {
    for (byte_t *p = s_ptr; p <= e_ptr; ++p) {
        *p = 9 - *p;
    }

    decArrAddNum(s_ptr, e_ptr, 1);
}

byte_t *getSumDecArr(byte_t *s_ptr_one, byte_t *s_ptr_two) {
    byte_t *res = (byte_t *) calloc(ARR_SIZE, sizeof(byte_t));
    int order_one = scanDecArrToSignedNum(s_ptr_one + P_ORDER, s_ptr_one + P_M - 1);
    int order_two = scanDecArrToSignedNum(s_ptr_two + P_ORDER, s_ptr_two + P_M - 1);
    int res_order = 0;

    // выравнивание порядков
    int additional = order_one - order_two;
    if (additional < 0) {
        shiftDecArr(s_ptr_one + P_M, s_ptr_one + P_END, additional);
        writeSignedNumToDecArr(s_ptr_one + P_ORDER, s_ptr_one + P_M - 1, order_two);
        res_order = order_two;
    } else {
        shiftDecArr(s_ptr_two + P_M, s_ptr_two + P_END, -1 * additional);
        writeSignedNumToDecArr(s_ptr_two + P_ORDER, s_ptr_two + P_M - 1, order_one);
        res_order = order_one;
    }

    // ищем общий индекс ппоследней значащей цифры
    int end_index = 0;
    for (int i = P_END; i >= P_M && end_index == 0; --i) {
        if (s_ptr_one[i] != 0 || s_ptr_two[i] != 0) {
            end_index = i;
        }
    }

    // записываем первое число в новый массив
    for (int i = 0; i <= P_END - P_M && end_index - i >= P_M; ++i) {
        res[P_END - i] = s_ptr_one[end_index - i];
    }

    // если число отрицательное, то превращаем его в доп. код (всё число)
    if (s_ptr_one[P_ZN] == 9) {
        negDecArr(res, res + P_END);
    }

    int buffer = 0;
    int index_num_two = end_index;
    if (s_ptr_two[P_ZN] == 9) {
        buffer = 1;

        for (int i = P_END; i >= 0; --i) {
            if (index_num_two < P_M) {
                buffer += res[i] + 9;
            } else {
                buffer += res[i] + (9 - s_ptr_two[--index_num_two]);
            }

            res[i] = buffer % 10;
            buffer /= 10;
        }
    } else {
        for (int i = P_END; i >= 0; --i) {
            if (index_num_two < P_M) {
                if (buffer == 0) {
                    break;
                }

                buffer += res[i];
            } else {
                buffer += res[i] + s_ptr_two[index_num_two--];
            }

            res[i] = buffer % 10;
            buffer /= 10;
        }
    }

    // если результат получился отрицательным - инвертируем
    if (res[P_ZN] == 9) {
        negDecArr(res, res + P_END);
        res[P_ZN] = 9;
    }

    // восстанавливаем прежний порядок числа
    shiftDecArr(res + P_M, res + P_END, P_END - end_index);

    // выравниваем порядки
    int shift = -1;
    while (res[P_M - 1 - ++shift] != 0) {
        ++res_order;
    }
    shiftDecArr(res, res + P_END, shift);
    writeSignedNumToDecArr(res + P_ORDER, res + P_M - 1, res_order);

    return res;
}
