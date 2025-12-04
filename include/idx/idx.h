#ifndef IDX_IDX_H_INCLUDED
#define IDX_IDX_H_INCLUDED
#include <stddef.h>
#include <stdint.h>
#include "idx/abi.h"

typedef struct idx_header IdxHeader;

struct idx_header {
    uint8_t type;
    uint8_t ndims;
    uint32_t lengths[256];
    size_t nelems;
    size_t nbytes;
    int bodystart;
};

IDX_ABI const char * idx_get_type_name (const IdxHeader * header);
IDX_ABI void idx_read_body_as_double (const char * path, const IdxHeader * header, double * buffer);
IDX_ABI void idx_read_body_as_float (const char * path, const IdxHeader * header, float * buffer);
IDX_ABI void idx_read_body_as_int8 (const char * path, const IdxHeader * header, int8_t * buffer);
IDX_ABI void idx_read_body_as_int16 (const char * path, const IdxHeader * header, int16_t * buffer);
IDX_ABI void idx_read_body_as_int32 (const char * path, const IdxHeader * header, int32_t * buffer);
IDX_ABI void idx_read_body_as_uint8 (const char * path, const IdxHeader * header, uint8_t * buffer);
IDX_ABI IdxHeader idx_read_header (const char * path);

#endif
