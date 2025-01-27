#ifndef PROTECTED_H
#define PROTECTED_H
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>


void read_dimension_lengths(FILE * fp, size_t ndims, uint32_t * lengths, size_t * nelems);
uint8_t read_ndims (FILE * fp);
uint8_t read_type (FILE * fp);
uint8_t read_uint8(FILE * fp, size_t pos);
void swap_byte_order_in_place(size_t n, uint8_t * bytes);
void validate_magic_number (FILE * fp);

#endif
