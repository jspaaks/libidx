#include <criterion/criterion.h>
#include "idx/division.h"

Test(idx_division, one){
    int actual = idx_divide(2, 3);
    int expected = 0;
    cr_assert(actual == expected);
}

Test(idx_division, two){
    int actual = idx_divide(2, -3);
    int expected = 0;
    cr_assert(actual == expected);
}
