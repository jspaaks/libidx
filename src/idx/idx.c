#include "protected.h"
#include "idx/idx.h"
#include <assert.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static const char * idx_type_names[16] = {
    [0x08] = "uint8",
    [0x09] = "int8",
    [0x0b] = "int16",
    [0x0c] = "int32",
    [0x0d] = "float",
    [0x0e] = "double",
};


const char * idx_get_type_name (const IdxHeader * header) {
    return idx_type_names[header->type];
}


void * idx_read_body (const char * path, const IdxHeader * header) {
    const uint8_t widths[16] = {
        [0x08] = 1,
        [0x09] = 1,
        [0x0b] = 2,
        [0x0c] = 4,
        [0x0d] = 4,
        [0x0e] = 8
    };

    const uint8_t width = widths[header->type];
    const size_t nbytes = width * header->nelems;
    errno = 0;
    uint8_t * body = calloc(nbytes * sizeof(uint8_t), 1);
    if (body == nullptr) {
        fprintf(stderr, "%s\nError allocating memory for body, aborting.\n", strerror(errno));
        errno = 0;
        exit(EXIT_FAILURE);
    }
    errno = 0;
    FILE * fp = fopen(path, "rb");
    fseek(fp, header->bodystart, SEEK_SET);
    if (fp == nullptr) {
        fprintf(stderr, "%s\nError reading binary data from file '%s', aborting.\n", strerror(errno), path);
        errno = 0;
        exit(EXIT_FAILURE);
    }
    const size_t count = fread(body, 1, nbytes, fp);
    if (count != nbytes) {
        fprintf(stderr, "Something went wrong reading data from file \"%s\", aborting.\n", path);
        fclose(fp);
        exit(EXIT_FAILURE);
    }
    fclose(fp);
    if (width > 1) {
        for (size_t i = 0; i < header->nelems; i++) {
            swap_byte_order_in_place(width, &body[i * width]);
        }
    }
    return (void *) body;
}


double * idx_read_body_as_double (const char * path, const IdxHeader * header) {
    // correct type casting depends on the data being exactly 8 bytes wide
    assert(sizeof(double) == 8 && "Unexpected size for data type double, aborting.\n");
    return (double *) idx_read_body (path, header);
}


float * idx_read_body_as_float (const char * path, const IdxHeader * header) {
    // correct type casting depends on the data being exactly 4 bytes wide
    assert(sizeof(float) == 4 && "Unexpected size for data type float, aborting.\n");
    return (float *) idx_read_body (path, header);
}


int8_t * idx_read_body_as_int8 (const char * path, const IdxHeader * header) {
    return (int8_t *) idx_read_body (path, header);
}


int16_t * idx_read_body_as_int16 (const char * path, const IdxHeader * header) {
    return (int16_t *) idx_read_body (path, header);
}


int32_t * idx_read_body_as_int32 (const char * path, const IdxHeader * header) {
    return (int32_t *) idx_read_body (path, header);
}


uint8_t * idx_read_body_as_uint8 (const char * path, const IdxHeader * header) {
    return (uint8_t *) idx_read_body (path, header);
}


IdxHeader idx_read_header (const char * path) {
    errno = 0;
    FILE * fp = fopen(path, "rb");
    if (fp == nullptr) {
        fprintf(stderr, "%s\nError reading binary data from file '%s', aborting.\n", strerror(errno), path);
        errno = 0;
        exit(EXIT_FAILURE);
    }
    // dimension lengths are stored as 32 bit unsigned integers (most significant byte first),
    // starting at byte at index 2 + 1 + 1 = 4
    const uint8_t width = 4;
    validate_magic_number(fp);
    IdxHeader header = {};
    header.type = read_type(fp);
    header.ndims = read_ndims (fp);
    header.bodystart = 2 + 1 + 1 + header.ndims * width;
    const size_t nbytes = header.ndims * width;
    fseek(fp, 4, SEEK_SET);
    size_t count = fread(&header.lengths[0], 1, nbytes, fp);
    if (count != nbytes) {
        fprintf(stderr, "Something went wrong reading dimension lengths, aborting.\n");
        fclose(fp);
        exit(EXIT_FAILURE);
    }
    header.nelems = 1;
    for (size_t i = 0; i < header.ndims; i++) {
        swap_byte_order_in_place(width, (uint8_t *) &header.lengths[i]);
        header.nelems *= header.lengths[i];
    }
    fclose(fp);
    return header;
}
