#include <stdio.h>
#include <math.h>
#include "idx/division.h"
#include "idx/multiplication.h"

int main (void) {
    fprintf(stdout, "-- test compile definitions\n");
#ifndef DEBUG
    fprintf(stdout, "   DEBUG compile definition has not been defined.\n");
#else
    fprintf(stdout, "   DEBUG compile definition has been defined.\n");
#endif
    fprintf(stdout, "\n");

    fprintf(stdout, "-- test whether math library was linked\n");
    fprintf(stdout, "   sqrt(144) = %f\n", sqrt(144));
    fprintf(stdout, "\n");

    fprintf(stdout, "-- test own library\n");
    fprintf(stdout, "   idx_divide(2, 3) = %d\n", idx_divide(2, 3));
    fprintf(stdout, "   idx_multiply(2, 3) = %d\n", idx_multiply(2, 3));
    fprintf(stdout, "\n");

    return 0;
}
