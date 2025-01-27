#ifndef BODIES_H
#define BODIES_H
#include "idx/idx.h"
#include <stdio.h>

void read_as_float_and_print(const char * path, IdxHeader header);
void read_as_int8_and_print(const char * path, IdxHeader header);
void read_as_int16_and_print(const char * path, IdxHeader header);
void read_as_int32_and_print(const char * path, IdxHeader header);
void read_as_uint8_and_print(const char * path, IdxHeader header);
void show_usage(FILE * stream);

#endif
