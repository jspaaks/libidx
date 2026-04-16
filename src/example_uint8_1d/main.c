#include "idx/idx.h"     // idx_*, IDX_*, Idx_*
#include <inttypes.h>    // PRIu8
#include <stdint.h>      // uint8_t
#include <stdio.h>       // fprintf, stderr, stdout
#include <stdlib.h>      // EXIT_SUCCESS, EXIT_FAILURE, exit, calloc, free
#include <string.h>      // strcmp

// function prototypes
static void show_usage(FILE * stream, const char * programname);

int main (const int argc, const char * argv[]) {

    // verify the user is using the program as intended
    if (argc != 2) {
        show_usage(stderr, argv[0]);
        exit(EXIT_FAILURE);
    }

    // show the help if the user asked for it
    if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
        show_usage(stdout, argv[0]);
        exit(EXIT_SUCCESS);
    }

    // create the IDX file object
    const char * filename = argv[1];
    IdxFileObject * o = idx_create(filename);
    idx_read_meta(o);
    idx_read_body(o);
    // verify the data has the right number of dimensions
    if (idx_get_ndims(o) != 1) {
        fprintf(stderr, "Expected 1 dimension, aborting.\n");
        return EXIT_FAILURE;
    };

    // verify that the data is uint8_t and get a pointer to it
    const uint8_t * mem = idx_get_data_uint8(o);

    // get the number of elements
    const int nelems = idx_get_dim_length(o, 0);

    // print the matrix
    for (int i = 0; i < nelems; i++) {
        char c = i == nelems - 1 ? '\n' : ' ';
        fprintf(stdout, "%3" PRIu8 "%c", mem[i], c);
    }

    // free idx memory resources
    idx_destroy(&o);

    // exit the program with a success code
    return EXIT_SUCCESS;
}

void show_usage(FILE * stream, const char * programname) {
    fprintf(stream, "Usage: %s FILEPATH\n"
                    "\n"
                    "   Illustrate the usage of the idx library by reading IDX-formatted data\n"
                    "   from a binary file located at FILEPATH. The 3rd byte in the file should\n"
                    "   be 0x08 to indicate that the body consists of 8-bit unsigned integer values,\n"
                    "   and the 4th byte should be 1 to indicate that there is just 1 dimension.\n"
                    "\n"
                    "   The program reads the data from file and prints the result to standard\n"
                    "   output.\n\n", programname);
}
