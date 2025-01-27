#include "strategies.h"
#include "idx/idx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void print_header(const IdxHeader * header) {
    fprintf(stdout,
           "header: {\n"
           "    type: 0x%02hhx,\n"
           "    ndims: %hhu,\n"
           "    lengths: [],\n"
           "    nelems: %zu,\n"
           "    bodystart: %d\n"
           "}\n", header->type, header->ndims, header->nelems, header->bodystart);
}

void strategy_doubles (const char * path, const IdxHeader * header) {
    print_header(header);
    double * body = idx_read_body_as_double(path, header);
    fprintf(stdout, "body: [");
    for (size_t i = 0; i < header->nelems; i++) {
        fprintf(stdout, "\n    % lg", body[i]);
    }
    fprintf(stdout, "%s", header->nelems == 0 ? "]\n" : "\n]\n");
    free(body);
}


void strategy_floats (const char * path, const IdxHeader * header) {
    print_header(header);
    float * body = idx_read_body_as_float(path, header);
    fprintf(stdout, "body: [");
    for (size_t i = 0; i < header->nelems; i++) {
        fprintf(stdout, "\n    % lg", body[i]);
    }
    fprintf(stdout, "%s", header->nelems == 0 ? "]\n" : "\n]\n");
    free(body);
}


void strategy_int8s (const char * path, const IdxHeader * header) {
    print_header(header);
    int8_t * body = idx_read_body_as_int8(path, header);
    fprintf(stdout, "body: [");
    for (size_t i = 0; i < header->nelems; i++) {
        fprintf(stdout, "\n    % hhi", body[i]);
    }
    fprintf(stdout, "%s", header->nelems == 0 ? "]\n" : "\n]\n");
    free(body);
}


void strategy_int16s (const char * path, const IdxHeader * header) {
    print_header(header);
    int16_t * body = idx_read_body_as_int16(path, header);
    fprintf(stdout, "body: [");
    for (size_t i = 0; i < header->nelems; i++) {
        fprintf(stdout, "\n    % hi", body[i]);
    }
    fprintf(stdout, "%s", header->nelems == 0 ? "]\n" : "\n]\n");
    free(body);
}


void strategy_int32s (const char * path, const IdxHeader * header) {
    print_header(header);
    int32_t * body = idx_read_body_as_int32(path, header);
    fprintf(stdout, "body: [");
    for (size_t i = 0; i < header->nelems; i++) {
        fprintf(stdout, "\n    % d", body[i]);
    }
    fprintf(stdout, "%s", header->nelems == 0 ? "]\n" : "\n]\n");
    free(body);
}


void strategy_uint8s (const char * path, const IdxHeader * header) {
    print_header(header);
    uint8_t * body = idx_read_body_as_uint8(path, header);
    fprintf(stdout, "body: [");
    for (size_t i = 0; i < header->nelems; i++) {
        fprintf(stdout, "\n    %hhu", body[i]);
    }
    fprintf(stdout, "%s", header->nelems == 0 ? "]\n" : "\n]\n");
    free(body);
}
