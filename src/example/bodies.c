#include "bodies.h"
#include "idx/idx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void read_int8_and_print(const char * path, IdxHeader header) {
    int8_t * body = idx_read_body_as_int8(path, header);
    for (size_t i = 0; i < header.nelems; i++) {
        fprintf(stdout, "%hhi ", body[i]);
    }
    fprintf(stdout, "\n");
    free(body);
}


void read_int16_and_print(const char * path, IdxHeader header) {
    int16_t * body = idx_read_body_as_int16(path, header);
    for (size_t i = 0; i < header.nelems; i++) {
        fprintf(stdout, "%hi ", body[i]);
    }
    fprintf(stdout, "\n");
    free(body);
}


void read_int32_and_print(const char * path, IdxHeader header) {
    int32_t * body = idx_read_body_as_int32(path, header);
    for (size_t i = 0; i < header.nelems; i++) {
        fprintf(stdout, "%d ", body[i]);
    }
    fprintf(stdout, "\n");
    free(body);
}


void read_uint8_and_print(const char * path, IdxHeader header) {
    uint8_t * body = idx_read_body_as_uint8(path, header);
    for (size_t i = 0; i < header.nelems; i++) {
        fprintf(stdout, "%hhu ", body[i]);
    }
    fprintf(stdout, "\n");
    free(body);
}
