#include <criterion/criterion.h>
#include "idx/multiplication.h"

Test(idx_multiplication, one){
    int actual = idx_multiply(2, 3);
    int expected = 6;
    cr_assert(actual == expected);
}

Test(idx_multiplication, two){
    int actual = idx_multiply(2, -3);
    int expected = -6;
    cr_assert(actual == expected);
}
