#include "bodies.h"
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
    IdxHeader header = idx_read_header(path);
    switch (header.type) {
        case 0x08: {
            read_as_uint8_and_print(path, header);
            break;
        }
        case 0x09: {
            read_as_int8_and_print(path, header);
            break;
        }
        case 0x0b: {
            read_as_int16_and_print(path, header);
            break;
        }
        case 0x0c: {
            read_as_int32_and_print(path, header);
            break;
        }
        case 0x0d: {
            read_as_float_and_print(path, header);
            break;
        }
        default: {
            fprintf(stderr, "Unexpected type, aborting.\n");
            exit(EXIT_FAILURE);
        }
    }
    return EXIT_SUCCESS;
}


void show_usage(FILE * stream) {
    fprintf(stream, "Usage: ./example-idx FILEPATH\n"
                    "   Illustrate the usage of the idx library by reading IDX-formatted\n"
                    "   data from a binary file located at FILEPATH.\n");
}
