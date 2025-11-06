#ifndef IDX_IDX_H_INCLUDED
#define IDX_IDX_H_INCLUDED
#include <stddef.h>
#include <stdint.h>
#include "idx/export.h"

typedef struct idx_header IdxHeader;

struct idx_header {
    uint8_t type;
    uint8_t ndims;
    uint32_t lengths[256];
    size_t nelems;
    int bodystart;
};

// core api
IDX_EXPORT void idx_free_body (void ** body);
IDX_EXPORT IdxHeader idx_read_header (const char * path);
IDX_EXPORT void * idx_read_body (const char * path, const IdxHeader * header);
IDX_EXPORT const char * idx_get_type_name (const IdxHeader * header);

// type-casting wrappers around idx_read_body
IDX_EXPORT double * idx_read_body_as_double (const char * path, const IdxHeader * header);
IDX_EXPORT float * idx_read_body_as_float (const char * path, const IdxHeader * header);
IDX_EXPORT int8_t * idx_read_body_as_int8 (const char * path, const IdxHeader * header);
IDX_EXPORT int16_t * idx_read_body_as_int16 (const char * path, const IdxHeader * header);
IDX_EXPORT int32_t * idx_read_body_as_int32 (const char * path, const IdxHeader * header);
IDX_EXPORT uint8_t * idx_read_body_as_uint8 (const char * path, const IdxHeader * header);

#endif
