#include <stdio.h>
#include "decArr/decArr.h"

int main() {
    byte_t *a = getDecArrFromStdin();
    DEC_ARR_PRINT(a);
    NEW_LINE;

    byte_t *b = getDecArrFromStdin();
    DEC_ARR_PRINT(b);
    NEW_LINE;

    byte_t *c = MULTIPLEXER_ADD(a, b);
    DEC_ARR_PRINT(c);
    NEW_LINE;

    const char *res_str = getNumFromDecArr(c);
    S_PRINT(res_str);
    NEW_LINE;

    free(a);
    free(b);
    free(c);
    free((void *) res_str);

    return 0;
}
