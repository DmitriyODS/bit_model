#include <stdio.h>

#include "decArr/decArr.h"

int main() {
    byte_t *a = getDecArrFromStdin();
    DEC_ARR_PRINT(a);
    NEW_LINE;

    byte_t *b = getDecArrFromStdin();
    DEC_ARR_PRINT(b);
    NEW_LINE;

    byte_t *c = getSumDecArr(a, b);
    DEC_ARR_PRINT(c);
    NEW_LINE;

    S_PRINT(getNumFromDecArr(c));
    NEW_LINE;

//    printf("%d\n", 12 % 10);
//    printf("%d\n", -12 % 10);
//    printf("%d\n", 12 / 10);
//    printf("%d\n", -3 / 10);

    return 0;
}
