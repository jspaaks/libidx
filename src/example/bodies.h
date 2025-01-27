#ifndef BODIES_H
#define BODIES_H
#include "idx/idx.h"
#include <stdio.h>

void read_int8_and_print(const char * path, IdxHeader header);
void read_int16_and_print(const char * path, IdxHeader header);
void read_uint8_and_print(const char * path, IdxHeader header);
void show_usage(FILE * stream);

#endif
