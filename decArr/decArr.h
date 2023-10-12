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

#define ARR_SIZE 30
#define M_SIZE 24
#define O_SIZE 5

#define P_ZN 0
#define P_ORDER 1
#define P_M 6

#define NEW_LINE printf("\n")
#define D_PRINT(data) printf("%d", data)
#define S_PRINT(data) printf("%s", data)
#define DEC_ARR_PRINT(data) printDecArr(data, data + P_END)

#define P_END 29

typedef unsigned char byte_t;
typedef byte_t dec_arr_t[30];

// ввод числа с клавиатуры
byte_t *getDecArrFromStdin();

// запись числа со знаком в десятичный массив
// s_ptr - указатель на первый элемент
// e_ptr - указатель на последний элемент
void writeSignedNumToDecArr(const byte_t *s_ptr, byte_t *e_ptr, int num);

// считывание числа со знаком из десятичного массива в число
// s_ptr - указатель на первый элемент
// e_ptr - указатель на последний элемент
int scanDecArrToSignedNum(const byte_t *s_ptr, const byte_t *e_ptr);

// вывод decArr на экран
// s_ptr - указатель на первый элемент
// e_ptr - указатель на последний элемент
void printDecArr(byte_t *s_ptr, const byte_t *e_ptr);

// преобразование decArr to int
const char *getNumFromDecArr(const byte_t *s_ptr);

// суммирование чисел
byte_t *getSumDecArr(byte_t *s_ptr_one, byte_t *s_ptr_two);

// вычитание чисел
byte_t *getSubDecArr(byte_t *s_ptr_one, byte_t *s_ptr_two);

// сдвиг значений на заданную величину
// shift < 0 - сдвиг вправо
// shift > 0 - сдвиг влево
void shiftDecArr(byte_t *s_ptr, byte_t *e_ptr, int shift);

// сложение с положительным числом
void decArrAddNum(const byte_t *s_ptr, byte_t *e_ptr, unsigned int num);

// конвертирвоание в дополнительный код
void negDecArr(byte_t *s_ptr, byte_t *e_ptr);


#endif // DEC_ARR_H
