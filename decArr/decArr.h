/*
 * decArr - библиотека для моделирвоания процессов сложения и вычитания
 * целых и вещественных чисел 10-й формы в экспоненциальном виде.
 * Поддерживаемая длинна числа до 30 знаков.
 *
 * Все числа (целые и вещественные) хранятся в форме: (zn)0.mEN, где
 * - zn - знак числа
 * - m - мантисса (само число)
 * - N - порядок (маштаб числа)
 * - E - основание (всегда 10)
 *
 * [(zn)] + [m] + [n] <= 30
 *
 * Числа храняться в десятичном массиве вида:
 * [0 00000 000000000000000000000000]
 * где первая часть - знак числа
 * вторая часть - порядок
 * третья часть - мантисса
 * */

#ifndef DEC_ARR_H
#define DEC_ARR_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#define ALL_SIZE 30
#define M_SIZE 24
#define E_SIZE 5

#define P_ZN 0
#define P_EXPONENT 1
#define P_M 6

#define P_START 0
#define P_END 29

#define SHIFT_EXPONENT 49999

#define BUFFER_SIZE 25

#define NEW_LINE printf("\n")
#define D_PRINT(data) printf("%d", data)
#define S_PRINT(data) printf("%s", data)
#define DEC_ARR_PRINT(data) printDecArr(data, data + P_END)

#define MULTIPLEXER_ADD(num_1, num_2) multiplexer(num_1, num_2, false)
#define MULTIPLEXER_SUB(num_1, num_2) multiplexer(num_1, num_2, true)

typedef unsigned char byte_t;
typedef byte_t dec_arr_t[30];

// ввод числа с клавиатуры
byte_t *getDecArrFromStdin();

// вывод decArr на экран
// s_ptr - указатель на первый элемент
// e_ptr - указатель на последний элемент
void printDecArr(byte_t *s_ptr, const byte_t *e_ptr);

// преобразование decArr to int
const char *getNumFromDecArr(const byte_t *p_num);

// сложение с положительным числом
void decArrAddNum(const byte_t *s_ptr, byte_t *e_ptr, unsigned int num);

// конвертирвоание в дополнительный код
void negDecArr(byte_t *s_ptr, byte_t *e_ptr);

// запись порядка со смещением
void writeExponent(byte_t *p_num, int exponent);

// чтение порядка со смещением
int scanExponent(const byte_t *p_num);

// сравнение порядков
int cmpExponent(const byte_t *p_one, const byte_t *p_two);

// получаем копию мантиссы со сдвигом
byte_t *getShiftMantissa(const byte_t *p_num, int shift);

// сложение/вычитание мантисс
byte_t *multiplexer(byte_t *p_one, byte_t *p_two, bool is_sub);

// нормализация числа
void normalize(byte_t *p_num, int exp);


#endif // DEC_ARR_H
