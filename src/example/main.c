#include "strategies.h"
#include "idx/idx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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
    const Strategy strategies[16] = {
        [0x08] = strategy_uint8s,
        [0x09] = strategy_int8s,
        [0x0b] = strategy_int16s,
        [0x0c] = strategy_int32s,
        [0x0d] = strategy_floats,
        [0x0e] = strategy_doubles
    };
    const IdxHeader header = idx_read_header(path);
    strategies[header.type](path, &header);
    return EXIT_SUCCESS;
}


void show_usage(FILE * stream) {
    fprintf(stream, "Usage: ./example-idx FILEPATH\n"
                    "   Illustrate the usage of the idx library by reading IDX-formatted\n"
                    "   data from a binary file located at FILEPATH.\n");
}
