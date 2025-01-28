#ifndef IDX_H
#define IDX_H
#include <stddef.h>
#include <stdint.h>

typedef struct idx_header IdxHeader;

struct idx_header {
    uint8_t type;
    uint8_t ndims;
    uint32_t lengths[256];
    size_t nelems;
    int bodystart;
};

// core api
IdxHeader idx_read_header (const char * path);
void * idx_read_body (const char * path, const IdxHeader * header);
const char * idx_get_type_name (const IdxHeader * header);

// type-casting wrappers around idx_read_body
double * idx_read_body_as_double (const char * path, const IdxHeader * header);
float * idx_read_body_as_float (const char * path, const IdxHeader * header);
int8_t * idx_read_body_as_int8 (const char * path, const IdxHeader * header);
int16_t * idx_read_body_as_int16 (const char * path, const IdxHeader * header);
int32_t * idx_read_body_as_int32 (const char * path, const IdxHeader * header);
uint8_t * idx_read_body_as_uint8 (const char * path, const IdxHeader * header);

#endif
