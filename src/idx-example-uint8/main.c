#include "idx/idx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void print_body (const IdxHeader * header, const uint8_t * body);
void print_header(const IdxHeader * header);
void show_usage(FILE * stream);


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
    const IdxHeader header = idx_read_header(path);
    if (header.type != 0x08) {
        fprintf(stderr, "Expected data in file \"%s\" to be of type 0x08 (uint8) but fou"
                "nd 0x%02hhx (%s), aborting.\n", path, header.type, idx_get_type_name(&header));
        exit(EXIT_FAILURE);
    }
    print_header(&header);
    uint8_t * body = idx_read_body_as_uint8(path, &header);
    print_body(&header, body);
    idx_free_body((void **) &body);
    return EXIT_SUCCESS;
}


void print_body (const IdxHeader * header, const uint8_t * body) {
    fprintf(stdout, "body: [");
    for (size_t i = 0; i < header->nelems; i++) {
        fprintf(stdout, "\n    %hhu", body[i]);
    }
    fprintf(stdout, "%s", header->nelems == 0 ? "]\n" : "\n]\n");
}


void print_header (const IdxHeader * header) {
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


void show_usage(FILE * stream) {
    fprintf(stream, "Usage: ./idx-example-uint8 FILEPATH\n"
                    "   Illustrate the usage of the idx library by reading IDX-formatted data\n"
                    "   from a binary file located at FILEPATH. The 3rd byte in the file should\n"
                    "   be 0x08 to indicate that the body consists of uint8 values.\n");
}
