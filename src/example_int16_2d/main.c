#include "idx/idx.h"     // idx_*, IDX_*, Idx_*
#include <inttypes.h>    // PRIi16
#include <stdint.h>      // int16_t
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
    IdxFileObject * o = idx_create_and_read(filename);

    // verify the data has the right number of dimensions
    if (idx_get_ndims(o) != 2) {
        fprintf(stderr, "Expected 2 dimensions, aborting.\n");
        return EXIT_FAILURE;
    };

    // verify that the data is int16_t and get a pointer to it
    const int16_t * mem = idx_get_data_int16(o);

    // allocate and initialize memory for accessing the data as a matrix
    const int nrows = idx_get_dim_length(o, 0);
    const int ncols = idx_get_dim_length(o, 1);
    const int16_t ** matrix = calloc(nrows, sizeof(int16_t *));
    for (int irow = 0; irow < nrows; irow++) {
        matrix[irow] = &mem[irow * ncols];
    }

    // print the matrix
    for (int irow = 0; irow < nrows; irow++) {
        for (int icol = 0; icol < ncols; icol++) {
            char c = icol == ncols - 1 ? '\n' : ' ';
            fprintf(stdout, "%6" PRIi16 "%c", matrix[irow][icol], c);
        }
    }

    // free idx memory resources
    idx_destroy(&o);

    // free other memory resources
    free(matrix);
    matrix = nullptr;

    // exit the program with a success code
    return EXIT_SUCCESS;
}

static void show_usage(FILE * stream, const char * programname) {
    fprintf(stream, "Usage: %s FILEPATH\n"
                    "\n"
                    "   Illustrate the usage of the idx library by reading IDX-formatted data\n"
                    "   from a binary file located at FILEPATH. The 3rd byte in the file should\n"
                    "   be 0x0B to indicate that the body consists of 16-bit signed integer values,\n"
                    "   and the 4th byte should be 2 to indicate that there are 2 dimensions.\n"
                    "\n"
                    "   The program reads the data from file and prints the result to standard\n"
                    "   output.\n\n", programname);
}
