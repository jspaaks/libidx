#include "idx/idx.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef void (*Strategy)(IdxFileObject * o);

static void show_usage (FILE * stream, const char * programname);
static void strat_double (IdxFileObject * o);
static void strat_float (IdxFileObject * o);
static void strat_int8 (IdxFileObject * o);
static void strat_int16 (IdxFileObject * o);
static void strat_int32 (IdxFileObject * o);
static void strat_uint8 (IdxFileObject * o);

int main (const int argc, const char * argv[]) {
    if (argc != 2) {
        show_usage(stderr, argv[0]);
        exit(EXIT_FAILURE);
    }
    if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
        show_usage(stdout, argv[0]);
        exit(EXIT_SUCCESS);
    }
    const char * filepath = argv[1];
    IdxFileObject * o = idx_create_and_read(filepath);

    const Strategy strategies[IDX_DATA_TYPE_DOUBLE + 1] = {
        [IDX_DATA_TYPE_UINT8] = strat_uint8,
        [IDX_DATA_TYPE_INT8] = strat_int8,
        [IDX_DATA_TYPE_INT16] = strat_int16,
        [IDX_DATA_TYPE_INT32] = strat_int32,
        [IDX_DATA_TYPE_FLOAT] = strat_float,
        [IDX_DATA_TYPE_DOUBLE] = strat_double
    };
    IdxDataType t = idx_get_type(o);
    strategies[t](o);

    idx_destroy(&o);

    return EXIT_SUCCESS;
}


static void show_usage (FILE * stream, const char * programname) {
    fprintf(stream, "Usage: %s FILEPATH\n"
                    "\n"
                    "    Read IDX-formatted data from a binary file located at FILEPATH.\n"
                    "\n", programname);
}

static void strat_double (IdxFileObject * o) {
    const double * buf = idx_get_data_double(o);
    if (idx_get_ndims(o) == 2) {
        int nrows = idx_get_dim_length(o, 0);
        int ncols = idx_get_dim_length(o, 1);
        const double ** matrix = calloc(nrows, sizeof(double *));
        if (matrix == nullptr) {
            fprintf(stderr, "Error allocating memory for 2-D array of double, aborting.\n");
            exit(EXIT_FAILURE);
        }
        for (int irow = 0; irow < nrows; irow++) {
            matrix[irow] = &buf[irow*ncols];
            for (int icol = 0; icol < ncols; icol++) {
                char c = icol < ncols - 1 ? ' ' : '\n';
                fprintf(stdout, "%11.4g%c", matrix[irow][icol], c);
            }
        }
        free(matrix);
        matrix = nullptr;
    } else {
        int nelems = idx_get_nelems(o);
        for (int i = 0; i < nelems; i++) {
            char c = i < nelems - 1 ? ' ' : '\n';
            fprintf(stdout, "%11.4g%c", buf[i], c);
        }
    }
}

static void strat_float (IdxFileObject * o) {
    const float * buf = idx_get_data_float(o);
    if (idx_get_ndims(o) == 2) {
        int nrows = idx_get_dim_length(o, 0);
        int ncols = idx_get_dim_length(o, 1);
        const float ** matrix = calloc(nrows, sizeof(float *));
        if (matrix == nullptr) {
            fprintf(stderr, "Error allocating memory for 2-D array of float, aborting.\n");
            exit(EXIT_FAILURE);
        }
        for (int irow = 0; irow < nrows; irow++) {
            matrix[irow] = &buf[irow*ncols];
            for (int icol = 0; icol < ncols; icol++) {
                char c = icol < ncols - 1 ? ' ' : '\n';
                fprintf(stdout, "%10.4g%c", matrix[irow][icol], c);
            }
        }
        free(matrix);
        matrix = nullptr;
    } else {
        int nelems = idx_get_nelems(o);
        for (int i = 0; i < nelems; i++) {
            char c = i < nelems - 1 ? ' ' : '\n';
            fprintf(stdout, "%10.4g%c", buf[i], c);
        }
    }
}

static void strat_int8 (IdxFileObject * o) {
    const int8_t * buf = idx_get_data_int8(o);
    if (idx_get_ndims(o) == 2) {
        int nrows = idx_get_dim_length(o, 0);
        int ncols = idx_get_dim_length(o, 1);
        const int8_t ** matrix = calloc(nrows, sizeof(int8_t *));
        if (matrix == nullptr) {
            fprintf(stderr, "Error allocating memory for 2-D array of int8_t, aborting.\n");
            exit(EXIT_FAILURE);
        }
        for (int irow = 0; irow < nrows; irow++) {
            matrix[irow] = &buf[irow*ncols];
            for (int icol = 0; icol < ncols; icol++) {
                char c = icol < ncols - 1 ? ' ' : '\n';
                fprintf(stdout, "%4" PRIi8 "%c", matrix[irow][icol], c);
            }
        }
        free(matrix);
        matrix = nullptr;
    } else {
        int nelems = idx_get_nelems(o);
        for (int i = 0; i < nelems; i++) {
            char c = i < nelems - 1 ? ' ' : '\n';
            fprintf(stdout, "%4" PRIi8 "%c", buf[i], c);
        }
    }
}

static void strat_int16 (IdxFileObject * o) {
    const int16_t * buf = idx_get_data_int16(o);
    if (idx_get_ndims(o) == 2) {
        int nrows = idx_get_dim_length(o, 0);
        int ncols = idx_get_dim_length(o, 1);
        const int16_t ** matrix = calloc(nrows, sizeof(int16_t *));
        if (matrix == nullptr) {
            fprintf(stderr, "Error allocating memory for 2-D array of int16_t, aborting.\n");
            exit(EXIT_FAILURE);
        }
        for (int irow = 0; irow < nrows; irow++) {
            matrix[irow] = &buf[irow*ncols];
            for (int icol = 0; icol < ncols; icol++) {
                char c = icol < ncols - 1 ? ' ' : '\n';
                fprintf(stdout, "%6" PRIi16 "%c", matrix[irow][icol], c);
            }
        }
        free(matrix);
        matrix = nullptr;
    } else {
        int nelems = idx_get_nelems(o);
        for (int i = 0; i < nelems; i++) {
            char c = i < nelems - 1 ? ' ' : '\n';
            fprintf(stdout, "%5" PRIi16 "%c", buf[i], c);
        }
    }
}

static void strat_int32 (IdxFileObject * o) {
    const int32_t * buf = idx_get_data_int32(o);
    if (idx_get_ndims(o) == 2) {
        int nrows = idx_get_dim_length(o, 0);
        int ncols = idx_get_dim_length(o, 1);
        const int32_t ** matrix = calloc(nrows, sizeof(int32_t *));
        if (matrix == nullptr) {
            fprintf(stderr, "Error allocating memory for 2-D array of int32_t, aborting.\n");
            exit(EXIT_FAILURE);
        }
        for (int irow = 0; irow < nrows; irow++) {
            matrix[irow] = &buf[irow*ncols];
            for (int icol = 0; icol < ncols; icol++) {
                char c = icol < ncols - 1 ? ' ' : '\n';
                fprintf(stdout, "%11" PRIi32 "%c", matrix[irow][icol], c);
            }
        }
        free(matrix);
        matrix = nullptr;
    } else {
        int nelems = idx_get_nelems(o);
        for (int i = 0; i < nelems; i++) {
            char c = i < nelems - 1 ? ' ' : '\n';
            fprintf(stdout, "%11" PRIi32 "%c", buf[i], c);
        }
    }
}

static void strat_uint8 (IdxFileObject * o) {
    const uint8_t * buf = idx_get_data_uint8(o);
    if (idx_get_ndims(o) == 2) {
        int nrows = idx_get_dim_length(o, 0);
        int ncols = idx_get_dim_length(o, 1);
        const uint8_t ** matrix = calloc(nrows, sizeof(uint8_t *));
        if (matrix == nullptr) {
            fprintf(stderr, "Error allocating memory for 2-D array of uint8_t, aborting.\n");
            exit(EXIT_FAILURE);
        }
        for (int irow = 0; irow < nrows; irow++) {
            matrix[irow] = &buf[irow*ncols];
            for (int icol = 0; icol < ncols; icol++) {
                char c = icol < ncols - 1 ? ' ' : '\n';
                fprintf(stdout, "%3" PRIu8 "%c", matrix[irow][icol], c);
            }
        }
        free(matrix);
        matrix = nullptr;
    } else {
        int nelems = idx_get_nelems(o);
        for (int i = 0; i < nelems; i++) {
            char c = i < nelems - 1 ? ' ' : '\n';
            fprintf(stdout, "%3" PRIu8 "%c", buf[i], c);
        }
    }
}
