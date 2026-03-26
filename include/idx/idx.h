#ifndef IDX_IDX_H_INCLUDED
#define IDX_IDX_H_INCLUDED
#include <stddef.h>       // size_t
#include <stdint.h>       // uint8_t, int8_t, int16_t, int32_t, uint32_t
#include "idx/export.h"   // IDX_EXPORT

struct idx_file_object;
typedef struct idx_file_object IdxFileObject;

typedef enum : uint8_t {
    IDX_DATA_TYPE_UINT8 = 0x08,     // char (unsigned byte)
    IDX_DATA_TYPE_INT8 = 0x09,      // char (signed byte)
    IDX_DATA_TYPE_INT16 = 0x0B,     // short (signed, 2 bytes)
    IDX_DATA_TYPE_INT32 = 0x0C,     // int (signed , 4 bytes)
    IDX_DATA_TYPE_FLOAT = 0x0D,     // float (signed, 4 bytes)
    IDX_DATA_TYPE_DOUBLE = 0x0E,    // double (signed, 8 bytes)
} IdxDataType;

// creating and destroying
IDX_EXPORT struct idx_file_object * idx_create_and_read (const char * filepath);
IDX_EXPORT void idx_destroy (struct idx_file_object ** self);

// metadata getters
IDX_EXPORT int idx_get_dim_length (const struct idx_file_object * self, int idim);
IDX_EXPORT uint32_t idx_get_dim_length_raw (const struct idx_file_object * self, int idim);
IDX_EXPORT int idx_get_ndims (const struct idx_file_object * self);
IDX_EXPORT uint8_t idx_get_ndims_raw (const struct idx_file_object * self);
IDX_EXPORT int idx_get_nelems (const struct idx_file_object * self);
IDX_EXPORT size_t idx_get_nelems_raw (const struct idx_file_object * self);
IDX_EXPORT IdxDataType idx_get_type (const struct idx_file_object * self);
IDX_EXPORT const char * idx_get_type_name (const struct idx_file_object * self);

// data pointer getters
IDX_EXPORT const double * idx_get_data_double (const struct idx_file_object * self);
IDX_EXPORT const float * idx_get_data_float (const struct idx_file_object * self);
IDX_EXPORT const int8_t * idx_get_data_int8 (const struct idx_file_object * self);
IDX_EXPORT const int16_t * idx_get_data_int16 (const struct idx_file_object * self);
IDX_EXPORT const int32_t * idx_get_data_int32 (const struct idx_file_object * self);
IDX_EXPORT const uint8_t * idx_get_data_uint8 (const struct idx_file_object * self);

#endif
