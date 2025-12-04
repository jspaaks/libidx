#include "idx/idx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef void (*Strategy)(const char *, const IdxHeader *, void * body_vp);

static void print_header(const IdxHeader * header);
static void show_usage(FILE * stream);
static void strategy_doubles (const char * path, const IdxHeader * header, void * body_vp);
static void strategy_floats (const char * path, const IdxHeader * header, void * body_vp);
static void strategy_int16s (const char * path, const IdxHeader * header, void * body_vp);
static void strategy_int32s (const char * path, const IdxHeader * header, void * body_vp);
static void strategy_int8s (const char * path, const IdxHeader * header, void * body_vp);
static void strategy_uint8s (const char * path, const IdxHeader * header, void * body_vp);


int main (const int argc, const char * argv[]) {
    if (argc != 2) {
        show_usage(stderr);
        exit(EXIT_FAILURE);
    }
    if (strncmp(argv[1], "-h", 3) == 0) {
        show_usage(stdout);
        exit(EXIT_SUCCESS);
    }
    const char * path = argv[1];
    const Strategy strategies[16] = {
        [0x08] = strategy_uint8s,
        [0x09] = strategy_int8s,
        [0x0b] = strategy_int16s,
        [0x0c] = strategy_int32s,
        [0x0d] = strategy_floats,
        [0x0e] = strategy_doubles
    };
    const IdxHeader header = idx_read_header(path);

    uint8_t * body = calloc(header.nbytes, 1);
    if (body == nullptr) {
        fprintf(stderr, "Something went wrong allocating dynamic memory for the data body, aborting.\n");
        exit(1);
    }
    strategies[header.type](path, &header, body);
    free(body);
    body = nullptr;

    return EXIT_SUCCESS;
}


static void print_header(const IdxHeader * header) {
    fprintf(stdout,
           "header: {\n"
           "    type: 0x%02hhx (%s),\n"
           "    ndims: %hhu,\n"
           "    lengths: [", header->type, idx_get_type_name(header), header->ndims);
    for (uint8_t i = 0; i < header->ndims; i++) {
        fprintf(stdout, "%s%u", i == 0 ? "" : ", ", header->lengths[i]);
    }
    fprintf(stdout,
           "],\n"
           "    nelems: %zu,\n"
           "    bodystart: %d\n"
           "}\n", header->nelems, header->bodystart);

}

static void show_usage(FILE * stream) {
    fprintf(stream, "Usage: ./idx-example-any-type FILEPATH\n"
                    "   Illustrate the usage of the idx library by reading IDX-formatted\n"
                    "   data from a binary file located at FILEPATH.\n");
}

static void strategy_doubles (const char * path, const IdxHeader * header, void * body_vp) {
    double * body = (double *) body_vp;
    print_header(header);
    idx_read_body_as_double(path, header, body);
    fprintf(stdout, "body: [");
    for (size_t i = 0; i < header->nelems; i++) {
        fprintf(stdout, "\n    % lg", body[i]);
    }
    fprintf(stdout, "%s", header->nelems == 0 ? "]\n" : "\n]\n");
}


static void strategy_floats (const char * path, const IdxHeader * header, void * body_vp) {
    float * body = (float *) body_vp;
    print_header(header);
    idx_read_body_as_float(path, header, body);
    fprintf(stdout, "body: [");
    for (size_t i = 0; i < header->nelems; i++) {
        fprintf(stdout, "\n    % lg", body[i]);
    }
    fprintf(stdout, "%s", header->nelems == 0 ? "]\n" : "\n]\n");
}


static void strategy_int8s (const char * path, const IdxHeader * header, void * body_vp) {
    int8_t * body = (int8_t *) body_vp;
    print_header(header);
    idx_read_body_as_int8(path, header, body);
    fprintf(stdout, "body: [");
    for (size_t i = 0; i < header->nelems; i++) {
        fprintf(stdout, "\n    % hhi", body[i]);
    }
    fprintf(stdout, "%s", header->nelems == 0 ? "]\n" : "\n]\n");
}


static void strategy_int16s (const char * path, const IdxHeader * header, void * body_vp) {
    int16_t * body = (int16_t *) body_vp;
    print_header(header);
    idx_read_body_as_int16(path, header, body);
    fprintf(stdout, "body: [");
    for (size_t i = 0; i < header->nelems; i++) {
        fprintf(stdout, "\n    % hi", body[i]);
    }
    fprintf(stdout, "%s", header->nelems == 0 ? "]\n" : "\n]\n");
}


static void strategy_int32s (const char * path, const IdxHeader * header, void * body_vp) {
    int32_t * body = (int32_t *) body_vp;
    print_header(header);
    idx_read_body_as_int32(path, header, body);
    fprintf(stdout, "body: [");
    for (size_t i = 0; i < header->nelems; i++) {
        fprintf(stdout, "\n    % d", body[i]);
    }
    fprintf(stdout, "%s", header->nelems == 0 ? "]\n" : "\n]\n");
}


static void strategy_uint8s (const char * path, const IdxHeader * header, void * body_vp) {
    uint8_t * body = (uint8_t *) body_vp;
    print_header(header);
    idx_read_body_as_uint8(path, header, body);
    fprintf(stdout, "body: [");
    for (size_t i = 0; i < header->nelems; i++) {
        fprintf(stdout, "\n    %hhu", body[i]);
    }
    fprintf(stdout, "%s", header->nelems == 0 ? "]\n" : "\n]\n");
}
