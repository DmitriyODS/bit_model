#include "decArr.h"

const char *enter_txt = "Enter your number in style: +/-123.45, max len number = 24 symbol without symbol +/- and dot. q - for stop enter->\n";

byte_t *getDecArrFromStdin() {
    byte_t *block = (byte_t *) calloc(ALL_SIZE, sizeof(byte_t));

    printf("%s", enter_txt);

    int buf = 0;
    int buf_order = 0;
    int exp = 0;
    byte_t count_missed = 0;
    byte_t pointer = P_M;
    bool is_dot = false;

    for (byte_t i = 0; i < M_SIZE; ++i) {
        buf = getchar();

        if (buf == EOF || buf == 'q' || buf == 'Q') {
            buf = getchar();
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
                ++exp;
            }
        } else if (buf > '0' && buf <= '9') {
            if (is_dot) {
                if (buf_order > -1) {
                    exp -= buf_order;
                    buf_order = -1;
                }
            } else {
                buf_order = -1;
                ++exp;
            }

            block[pointer++] = buf - '0';
        } else {
            ++count_missed;
        }
    }

    printf("Count missed invalid symbol = %d\n", count_missed);

    if (exp == 0 && block[P_M] == 0) {
        block[P_ZN] = 0;
    }

    writeExponent(block, exp);

    return block;
}

void printDecArr(byte_t *s_ptr, const byte_t *e_ptr) {
    for (byte_t *ptr = s_ptr; ptr <= e_ptr; ++ptr) {
        printf("%d", *ptr);
    }
}

const char *getNumFromDecArr(const byte_t *p_num) {
    // выделяем прям по полной, не хочу считать что - то
    char *res = (char *) calloc(ALL_SIZE + 1, sizeof(char));

    int exp = scanExponent(p_num);
    int e_ptr_num = 0;
    int s_write_ptr = 0;

    if (exp == 0 && p_num[P_M] == 0) {
        res[s_write_ptr++] = '0';
        res[s_write_ptr++] = '.';
        res[s_write_ptr++] = '0';

        return res;
    }

    if (p_num[P_ZN] == 9) {
        res[s_write_ptr++] = '-';
    }

    for (int i = ALL_SIZE - 1; i >= P_M; --i) {
        if (p_num[i] != 0 || P_M + exp - 1 == i) {
            e_ptr_num = i;
            break;
        }
    }

    if (exp < 1) {
        res[s_write_ptr++] = '0';
        res[s_write_ptr++] = '.';

        for (int i = 0; i < -exp; ++i) {
            res[s_write_ptr++] = '0';
        }

        for (int i = P_M; i <= e_ptr_num; ++i) {
            res[s_write_ptr++] = (char) (p_num[i] + '0');
        }
    } else {
        for (int i = P_M; i < P_M + exp; ++i) {
            res[s_write_ptr++] = (char) (p_num[i] + '0');
        }

        res[s_write_ptr++] = '.';
        res[s_write_ptr] = '0';

        for (int i = P_M + exp; i <= e_ptr_num; ++i) {
            res[s_write_ptr++] = (char) (p_num[i] + '0');
        }
    }

    return res;
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

void writeExponent(byte_t *p_num, int exponent) {
    int buf = exponent + SHIFT_EXPONENT;
    for (int i = E_SIZE; i >= P_EXPONENT; --i) {
        p_num[i] = buf % 10;
        buf /= 10;
    }
}

int scanExponent(const byte_t *p_num) {
    int res = 0;
    int cur_order = 1;
    for (int i = E_SIZE; i >= P_EXPONENT; --i) {
        res += p_num[i] * cur_order;
        cur_order *= 10;
    }

    return res - SHIFT_EXPONENT;
}

int cmpExponent(const byte_t *p_one, const byte_t *p_two) {
    return scanExponent(p_one) - scanExponent(p_two);
}

byte_t *getShiftMantissa(const byte_t *p_num, int shift) {
    byte_t *new_m = (byte_t *) calloc(M_SIZE, sizeof(byte_t));

    for (int i = shift; i < M_SIZE; ++i) {
        new_m[i] = p_num[P_M + i - shift];
    }

    return new_m;
}

void normalize(byte_t *p_num, int exp) {
    int index_start = P_M;
    while (p_num[index_start] == 0 && index_start <= P_END) {
        ++index_start;
    }

    if (index_start > P_END) {
        writeExponent(p_num, 0);
        return;
    }

    exp -= index_start - P_M;
    for (int i = index_start; i <= P_END; ++i) {
        p_num[P_M + i - index_start] = p_num[i];
    }

    writeExponent(p_num, exp);
}

byte_t *multiplexer(byte_t *p_one, byte_t *p_two, bool is_sub) {
    int sub_n = cmpExponent(p_one, p_two);
    int res_exponent = scanExponent(p_one);

    const byte_t *p_first_num = p_one;
    byte_t *p_second_num = p_two;

    // если N с минусом - меняем местами числа и записываем новый результирующий порядок числа
    if (sub_n < 0) {
        p_first_num = p_two;
        p_second_num = p_one;
        res_exponent = scanExponent(p_two);

        // сразу сделаем сдвиг положительным для дальнейших вычислений
        sub_n *= -1;
    }

    // если знак второго числа "-", то отражаем это в параметре
    if (p_second_num[P_ZN] == 9) {
        is_sub = !is_sub;
    }

    // получаем сдвинутую копию мантиссы второго числа
    p_second_num = getShiftMantissa(p_second_num, sub_n);

    // создаём буфер в котором будем выполнять сложение
    // размер буфера больше размера мантиссы на 1, чтобы поместить знак числа
    byte_t *buffer = (byte_t *) calloc(BUFFER_SIZE, sizeof(byte_t));

    // копируем первое число в буфер сложения
    for (int i = P_END; i >= P_M; --i) {
        buffer[i - P_M + 1] = p_first_num[i];
    }

    // если знак первого числа отрицателен - преобразовываем в доп код
    if (p_first_num[P_ZN] == 9) {
        negDecArr(buffer, buffer + BUFFER_SIZE - 1);
    }

    // выполняем сложение с учётом is_sub
    int transfert = 0;
    if (is_sub) {
        ++transfert;
    }

    for (int i = BUFFER_SIZE - 1; i > 0; --i) {
        if (is_sub) {
            transfert += buffer[i] + (9 - p_second_num[i - 1]);
        } else {
            transfert += buffer[i] + p_second_num[i - 1];
        }

        buffer[i] = transfert % 10;
        transfert /= 10;
    }

    // дополняем знаковый разряд
    transfert += buffer[0];
    if (is_sub) {
        transfert += 9;
    }
    buffer[0] = transfert % 10;

    // формируем итоговое число
    byte_t *res = (byte_t *) calloc(ALL_SIZE, sizeof(byte_t));

    // если знаковый разряд 9 - значит число отрицательное,
    // переводим из доп. кода и ставим минус в итоге
    if (buffer[0] == 9) {
        negDecArr(buffer, buffer + BUFFER_SIZE - 1);
        res[P_ZN] = 9;
    }

    // записываем новую мантиссу
    int buf_shift = 1;
    if (buffer[0] > 0) {
        buf_shift = 0;
        ++res_exponent;
    }
    for (int i = P_M; i <= P_END; ++i) {
        res[i] = buffer[i + buf_shift - P_M];
    }

    // нормализуем
    normalize(res, res_exponent);

    // освобождаем память
    free((void *) p_second_num);
    free((void *) buffer);
    p_second_num = NULL;
    buffer = NULL;

    return res;
}
