#ifndef STRATEGIES_H
#define STRATEGIES_H
#include "idx/idx.h"
#include <stdio.h>

typedef void (*Strategy)(const char *, const IdxHeader *);

void strategy_doubles (const char * path, const IdxHeader * header);
void strategy_floats (const char * path, const IdxHeader * header);
void strategy_int8s (const char * path, const IdxHeader * header);
void strategy_int16s (const char * path, const IdxHeader * header);
void strategy_int32s (const char * path, const IdxHeader * header);
void strategy_uint8s (const char * path, const IdxHeader * header);
void show_usage(FILE * stream);

#endif
