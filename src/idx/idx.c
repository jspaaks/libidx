#include "idx/idx.h"
#include <assert.h>
#include <errno.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <inttypes.h>


static uint8_t read_ndims (FILE * fp);
static uint8_t read_type (FILE * fp);
static uint8_t read_uint8(FILE * fp, size_t pos);
static void swap_byte_order_in_place (size_t n, uint8_t * bytes);
static void validate_magic_number (FILE * fp);

const char * idx_type_names[16] = {
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

void idx_read_body_as_double (const char * path, const IdxHeader * header, double * buffer) {
    // correct type casting depends on the data being exactly 8 bytes wide
    assert(sizeof(double) == 8 && "Unexpected size for data type double, aborting.\n");
    idx_read_body_as_uint8(path, header, (uint8_t *) buffer);
}


void idx_read_body_as_float (const char * path, const IdxHeader * header, float * buffer) {
    // correct type casting depends on the data being exactly 4 bytes wide
    assert(sizeof(float) == 4 && "Unexpected size for data type float, aborting.\n");
    idx_read_body_as_uint8(path, header, (uint8_t *) buffer);
}


void idx_read_body_as_int8 (const char * path, const IdxHeader * header, int8_t * buffer) {
    idx_read_body_as_uint8(path, header, (uint8_t *) buffer);
}


void idx_read_body_as_int16 (const char * path, const IdxHeader * header, int16_t * buffer) {
    idx_read_body_as_uint8(path, header, (uint8_t *) buffer);
}


void idx_read_body_as_int32 (const char * path, const IdxHeader * header, int32_t * buffer) {
    idx_read_body_as_uint8(path, header, (uint8_t *) buffer);
}


void idx_read_body_as_uint8 (const char * path, const IdxHeader * header, uint8_t * buffer) {

    // open the binary file from `path`
    errno = 0;
    FILE * fp = fopen(path, "rb");

    // use information from the header to skip to the beginning of the data body
    fseek(fp, header->bodystart, SEEK_SET);
    if (fp == nullptr) {
        fprintf(stderr, "%s\nError reading binary data from file '%s', aborting.\n", strerror(errno), path);
        errno = 0;
        exit(EXIT_FAILURE);
    }

    // read the complete body as uint8_t
    const size_t count = fread(buffer, 1, header->nbytes, fp);
    if (count != header->nbytes) {
        fprintf(stderr, "Something went wrong reading data from file \"%s\", aborting.\n", path);
        fclose(fp);
        exit(EXIT_FAILURE);
    }

    // close the file
    fclose(fp);

    // swap byte order if needed
    if (header->nbytes > header->nelems) {
        const size_t width = header->nbytes / header->nelems;
        for (size_t i = 0; i < header->nelems; i++) {
            swap_byte_order_in_place(width, &buffer[i * width]);
        }
    }
}


IdxHeader idx_read_header (const char * path) {

    // define the widths in number of bytes for each supported data type
    const uint8_t widths[16] = {
        [0x08] = 1,
        [0x09] = 1,
        [0x0b] = 2,
        [0x0c] = 4,
        [0x0d] = 4,
        [0x0e] = 8
    };
    FILE * fp = nullptr;
    IdxHeader header = {};

    // open the binary file at path
    {
        errno = 0;
        fp = fopen(path, "rb");
        if (fp == nullptr) {
            fprintf(stderr, "%s\nError reading binary data from file '%s', aborting.\n", strerror(errno), path);
            errno = 0;
            exit(EXIT_FAILURE);
        }
    }

    // validate that the file's first two bytes are correct
    {
        validate_magic_number(fp);
    }

    // read the identifier for the body's data type / width
    {
        header.type = read_type(fp);
    }

    // read the number of dimensions
    {
        header.ndims = read_ndims (fp);
    }

    // read length of each dimension; dimension lengths are stored as 32 bit unsigned integers
    // (most significant byte first), starting at byte at index 2 + 1 + 1 = 4; then, determine how
    // many data elements there are
    {
        const size_t width = 4; // dimensions are defined as uint32_t
        const uint8_t start = 4;
        const size_t nbytes = header.ndims * width;
        fseek(fp, start, SEEK_SET);
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
    }

    // don't need the file open anymore
    fclose(fp);

    // determine the number of bytes in the data body and at what byte the data body starts
    {
        header.nbytes = widths[header.type] * header.nelems;
        header.bodystart = 2 + 1 + 1 + header.ndims * 4;
    }

    return header;
}


static uint8_t read_ndims (FILE * fp) {
    // the byte at index 3 contains the number of dimensions
    return read_uint8(fp, 3);
}


static uint8_t read_type (FILE * fp) {
    // the byte at index 2 contains an indicator for the body's data type
    return read_uint8(fp, 2);
}


static uint8_t read_uint8(FILE * fp, const size_t pos) {
    // read the byte at index pos
    if (fseek(fp, pos, SEEK_SET) == -1) goto error;
    uint8_t result;
    size_t count = fread(&result, 1, 1, fp);
    if (count != 1) goto error;
    return result;
error:
    fprintf(stderr, "%s\nError reading byte, aborting.\n", strerror(errno));
    errno = 0;
    fclose(fp);
    exit(EXIT_FAILURE);
}


static void swap_byte_order_in_place(const size_t nbytes, uint8_t * bytes) {
    // swap the byte order of the next 'nbytes' bytes counting from 'bytes'
    uint8_t swap;
    if (nbytes % 2 != 0) {
        fprintf(stderr, "Expected an even number of bytes when swapping byte order, aborting.\n");
        exit(EXIT_FAILURE);
    }
    for (size_t i = 0; i < nbytes / 2; i++) {
        size_t j = nbytes - 1 - i;
        swap = bytes[i];
        bytes[i] = bytes[j];
        bytes[j] = swap;
    }
}


static void validate_magic_number (FILE * fp) {
    // validate that first two bytes are 0
    bool a = read_uint8(fp, 0) != 0;
    bool b = read_uint8(fp, 1) != 0;
    if (a || b) {
        fprintf(stderr, "Found unexpected value in first two bytes, aborting.\n");
        exit(EXIT_FAILURE);
    }
}
