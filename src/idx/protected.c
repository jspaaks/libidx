#include "protected.h"
#include <errno.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


uint8_t read_ndims (FILE * fp) {
    return read_uint8(fp, 3);
}


uint8_t read_type (FILE * fp) {
    return read_uint8(fp, 2);
}


uint8_t read_uint8(FILE * fp, const size_t pos) {
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


void swap_byte_order_in_place(const size_t nbytes, uint8_t * bytes) {
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


void validate_magic_number (FILE * fp) {
    // validate that first two bytes are 0
    bool a = read_uint8(fp, 0) != 0;
    bool b = read_uint8(fp, 1) != 0;
    if (a || b) {
        fprintf(stderr, "Found unexpected value in first two bytes, aborting.\n");
        exit(EXIT_FAILURE);
    }
}
