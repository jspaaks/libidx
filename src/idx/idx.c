#include "idx/idx.h"
#include <errno.h>
#include <inttypes.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>

// implementation-defined opaque type
struct idx_file_object {
    char * filepath;
    bool body_was_read;
    bool meta_was_read;
    IdxDataType type;
    uint8_t ndims;
    uint32_t * lengths;
    size_t nelems;
    uint8_t * buffer;
};

struct idx_type_prop {
    char * name;
    int width;
};

const struct idx_type_prop idx_type_props[IDX_DATA_TYPE_DOUBLE + 1] = {
    [IDX_DATA_TYPE_UINT8] = {.name = "uint8", .width = 1},
    [IDX_DATA_TYPE_INT8] = {.name = "int8", .width = 1},
    [IDX_DATA_TYPE_INT16] = {.name = "int16", .width = 2},
    [IDX_DATA_TYPE_INT32] = {.name = "int32", .width = 4},
    [IDX_DATA_TYPE_FLOAT] = {.name = "float", .width = 4},
    [IDX_DATA_TYPE_DOUBLE] = {.name = "double", .width = 8},
};

static void assert_meta_was_not_read (const struct idx_file_object * self);
static void assert_meta_was_read (const struct idx_file_object * self);
static void assert_body_was_not_read (const struct idx_file_object * self);
static void assert_body_was_read (const struct idx_file_object * self);
static size_t calc_nelems (const uint32_t * lengths, int ndims);
static void close_file (FILE * fp);
static FILE * open_file (const char * filepath);
static IdxDataType read_data_type (FILE * fp);
static uint32_t * read_dimension_lengths (FILE * fp, int ndims);
static uint8_t read_ndims (FILE * fp);
static uint8_t read_uint8 (FILE * fp, const size_t pos);
static void swap_byte_order_in_place (const int nbytes, uint8_t * bytes);
static void validate_magic_number (FILE * fp);

static void assert_meta_was_not_read (const struct idx_file_object * self) {
    if (self->meta_was_read) {
        fprintf(stderr, "ERROR: metadata has already been read, aborting.\n");
        exit(EXIT_FAILURE);
    }
}

static void assert_meta_was_read (const struct idx_file_object * self) {
    if (self->meta_was_read == false) {
        fprintf(stderr, "ERROR: metadata hasn't been read yet, aborting.\n");
        exit(EXIT_FAILURE);
    }
}


static void assert_body_was_not_read (const struct idx_file_object * self) {
    if (self->body_was_read) {
        fprintf(stderr, "ERROR: body has already been read, aborting.\n");
        exit(EXIT_FAILURE);
    }
}

static void assert_body_was_read (const struct idx_file_object * self) {
    if (self->body_was_read == false) {
        fprintf(stderr, "ERROR: body hasn't been read yet, aborting.\n");
        exit(EXIT_FAILURE);
    }
}

static size_t calc_nelems (const uint32_t * lengths, int ndims) {
    size_t nelems = 1;
    for (int idim = 0; idim < ndims; idim++) {
        nelems *= lengths[idim];
    }
    return nelems;
}

static void close_file (FILE * fp) {
    fclose(fp);
}

struct idx_file_object * idx_create (const char * filepath) {
    struct idx_file_object * o = calloc(1, sizeof(struct idx_file_object));
    if (o == nullptr) {
        fprintf(stderr, "Something went wrong allocating dynamic memory for `struct idx_file_object`, aborting.\n");
        exit(EXIT_FAILURE);
    }
    int cap = 256;
    int len = strlen(filepath) + 1;
    if (len > cap) {
        fprintf(stderr, "Parameter `filepath` too long (%d > %d), aborting.\n", len, cap);
        exit(EXIT_FAILURE);
    }
    char * filepath_copy = calloc(len, sizeof(char));
    if (filepath_copy == nullptr) {
        fprintf(stderr, "Something went wrong allocating dynamic memory for storing filepath, aborting.\n");
        exit(EXIT_FAILURE);
    }
    memcpy(filepath_copy, filepath, len - 1);
    filepath_copy[len - 1] = '\0';

    *o = (struct idx_file_object) {
        .body_was_read = false,
        .meta_was_read = false,
        .filepath = filepath_copy,
        .type = 0,
        .ndims = 0,
        .lengths = nullptr,
        .nelems = 0,
        .buffer = nullptr,
    };
    return o;
}

void idx_destroy (struct idx_file_object ** self) {

    free((*self)->filepath);
    (*self)->filepath = nullptr;

    free((*self)->lengths);
    (*self)->lengths = nullptr;

    free((*self)->buffer);
    (*self)->buffer = nullptr;

    free(*self);
    *self = nullptr;
}

int idx_get_dim_length (const struct idx_file_object * self, int idim) {
    assert_meta_was_read(self);
    uint32_t length = idx_get_dim_length_raw(self, idim);
    size_t w = sizeof(int) * 8;
    if (length <= INT_MAX) return (int) length;
    fprintf(stderr, "Dimension %d of length %" PRIu32 " is too large to fit in a %zu-bit int, "
                    "try using the *_raw counterpart\nof this function; aborting.\n", idim, length, w);
    exit(EXIT_FAILURE);
}

uint32_t idx_get_dim_length_raw (const struct idx_file_object * self, int idim) {
    assert_meta_was_read(self);
    if (0 <= idim && idim < self->ndims) {
        return self->lengths[idim];
    }
    fprintf(stderr, "Out of range value for `idim`, aborting.\n");
    exit(EXIT_FAILURE);
}

int idx_get_ndims (const struct idx_file_object * self) {
    assert_meta_was_read(self);
    // uint8_t always fits within int, can use casting directly
    return (int) idx_get_ndims_raw(self);
}

uint8_t idx_get_ndims_raw (const struct idx_file_object * self) {
    assert_meta_was_read(self);
    return self->ndims;
}

int idx_get_nelems (const struct idx_file_object * self) {
    assert_meta_was_read(self);
    size_t nelems = idx_get_nelems_raw(self);
    size_t w = sizeof(int) * 8;
    if (nelems <= INT_MAX) return (int) nelems;
    fprintf(stderr, "Number of elements (%zu) is too large to fit in a %zu-bit int, "
                    "try using *_raw counterpart\nof this function; aborting.\n", nelems, w);
    exit(EXIT_FAILURE);
}

size_t idx_get_nelems_raw (const struct idx_file_object * self) {
    assert_meta_was_read(self);
    return self->nelems;
}

IdxDataType idx_get_type (const struct idx_file_object * self) {
    assert_meta_was_read(self);
    return self->type;
}

const char * idx_get_type_name (const struct idx_file_object * self) {
    assert_meta_was_read(self);
    return idx_type_props[self->type].name;
}

const double * idx_get_data_double (const struct idx_file_object * self) {
    assert_meta_was_read(self);
    assert_body_was_read(self);
    if (self->type == IDX_DATA_TYPE_DOUBLE) {
        return (const double *) self->buffer;
    }
    fprintf(stderr, "Data is not in double format, aborting.\n");
    exit(EXIT_FAILURE);
}

const float * idx_get_data_float (const struct idx_file_object * self) {
    assert_meta_was_read(self);
    assert_body_was_read(self);
    if (self->type == IDX_DATA_TYPE_FLOAT) {
        return (const float *) self->buffer;
    }
    fprintf(stderr, "Data is not in float format, aborting.\n");
    exit(EXIT_FAILURE);
}

const int8_t * idx_get_data_int8 (const struct idx_file_object * self) {
    assert_meta_was_read(self);
    assert_body_was_read(self);
    if (self->type == IDX_DATA_TYPE_INT8) {
        return (const int8_t *) self->buffer;
    }
    fprintf(stderr, "Data is not in int8_t format, aborting.\n");
    exit(EXIT_FAILURE);
}

const int16_t * idx_get_data_int16 (const struct idx_file_object * self) {
    assert_meta_was_read(self);
    assert_body_was_read(self);
    if (self->type == IDX_DATA_TYPE_INT16) {
        return (const int16_t *) self->buffer;
    }
    fprintf(stderr, "Data is not in int16_t format, aborting.\n");
    exit(EXIT_FAILURE);
}

const int32_t * idx_get_data_int32 (const struct idx_file_object * self) {
    assert_meta_was_read(self);
    assert_body_was_read(self);
    if (self->type == IDX_DATA_TYPE_INT32) {
        return (const int32_t *) self->buffer;
    }
    fprintf(stderr, "Data is not in int32_t format, aborting.\n");
    exit(EXIT_FAILURE);
}

const uint8_t * idx_get_data_uint8 (const struct idx_file_object * self) {
    assert_meta_was_read(self);
    assert_body_was_read(self);
    if (self->type == IDX_DATA_TYPE_UINT8) {
        return (const uint8_t *) self->buffer;
    }
    fprintf(stderr, "Data is not in uint8_t format, aborting.\n");
    exit(EXIT_FAILURE);
}

void idx_read_body (struct idx_file_object * self) {
    assert_meta_was_read(self);
    assert_body_was_not_read(self);
    FILE * fp = open_file(self->filepath);

    // determine how much space we need given we want to store `nelems` data of type `type`
    int w = idx_type_props[self->type].width;
    const size_t nbytes = w * self->nelems;

    // allocate dynamic mmory for storing the body data
    errno = 0;
    uint8_t * body = calloc(nbytes * sizeof(uint8_t), 1);
    if (body == nullptr) {
        fprintf(stderr, "%s\nError allocating memory for IDX body, aborting.\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // put the cursor at the starting byte of the body
    int start = 2 + 1 + 1 + self->ndims * sizeof(uint32_t);
    fseek(fp, start, SEEK_SET);

    // read nbytes into `body`
    const size_t count = fread(body, 1, nbytes, fp);
    if (count != nbytes) {
        fprintf(stderr, "Something went wrong reading data from IDX file, aborting.\n");
        fclose(fp);
        exit(EXIT_FAILURE);
    }
    close_file(fp);

    // swap byte order for types of width > 1
    if (w > 1) {
        for (size_t i = 0; i < self->nelems; i++) {
            swap_byte_order_in_place(w, &body[i * w]);
        }
    }
    self->buffer = body;
    self->body_was_read = true;
}

void idx_read_meta (struct idx_file_object * self) {
    assert_meta_was_not_read(self);
    assert_body_was_not_read(self);

    FILE * fp = open_file(self->filepath);
    validate_magic_number(fp);
    uint8_t type = read_data_type(fp);
    uint8_t ndims = read_ndims(fp);
    uint32_t * lengths = read_dimension_lengths(fp, ndims);
    size_t nelems = calc_nelems(lengths, ndims);
    close_file(fp);

    self->meta_was_read = true;
    self->type = type;
    self->ndims = ndims;
    self->lengths = lengths;
    self->nelems = nelems;
}

static FILE * open_file(const char * filepath) {
    errno = 0;
    FILE * fp = fopen(filepath, "rb");
    if (fp == nullptr) {
        fprintf(stderr, "%s\nError reading binary data from file '%s', aborting.\n",
                strerror(errno), filepath);
        exit(EXIT_FAILURE);
    }
    return fp;
}

static IdxDataType read_data_type (FILE * fp) {
    // the byte at index 2 contains an indicator for the body's data type
    uint8_t val = read_uint8(fp, 2);
    switch (val) {
        case 0x08: return IDX_DATA_TYPE_UINT8;
        case 0x09: return IDX_DATA_TYPE_INT8;
        case 0x0B: return IDX_DATA_TYPE_INT16;
        case 0x0C: return IDX_DATA_TYPE_INT32;
        case 0x0D: return IDX_DATA_TYPE_FLOAT;
        case 0x0E: return IDX_DATA_TYPE_DOUBLE;
        default:
            fprintf(stderr, "IDX file has invalid data type, aborting.\n");
            fclose(fp);
            exit(EXIT_FAILURE);
    }
}

static uint32_t * read_dimension_lengths (FILE * fp, int ndims) {
    // allocate dynamic memory to store the length of all dimensions
    uint32_t * lengths = calloc(ndims, sizeof(uint32_t));
    if (lengths == nullptr) {
        fprintf(stderr, "Something went wrong allocating memory for dimension lengths, aborting.\n");
        fclose(fp);
        exit(EXIT_FAILURE);
    }

    // set the cursor to the position where the dimension lengths start
    fseek(fp, 4, SEEK_SET);

    // determine how many bytes to read
    const uint8_t width = sizeof(uint32_t);
    const size_t nbytes = ndims * width;

    // read nbytes bytes into uint32_t memory
    size_t count = fread(&lengths[0], 1, nbytes, fp);

    // check if the correct number of bytes was read
    if (count != nbytes) {
        fprintf(stderr, "Something went wrong reading dimension lengths, aborting.\n");
        fclose(fp);
        exit(EXIT_FAILURE);
    }

    // adjust the byte order
    for (int idim = 0; idim < ndims; idim++) {
        swap_byte_order_in_place(width, (uint8_t *) &lengths[idim]);
    }

    return lengths;
}

static uint8_t read_ndims (FILE * fp) {
    // the byte at index 3 contains the number of dimensions
    return read_uint8(fp, 3);
}

static uint8_t read_uint8 (FILE * fp, const size_t pos) {
    // read the byte at index pos
    errno = 0;
    if (fseek(fp, pos, SEEK_SET) == -1) goto error;
    uint8_t result;
    errno = 0;
    size_t count = fread(&result, 1, 1, fp);
    if (count != 1) goto error;
    return result;
error:
    fprintf(stderr, "%s\nError reading byte, aborting.\n", strerror(errno));
    fclose(fp);
    exit(EXIT_FAILURE);
}

static void swap_byte_order_in_place (const int nbytes, uint8_t * bytes) {
    // swap the byte order of the next 'nbytes' bytes counting from 'bytes'
    uint8_t swap;
    if (nbytes % 2 != 0) {
        fprintf(stderr, "Expected an even number of bytes when swapping byte order, aborting.\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < nbytes / 2; i++) {
        int j = nbytes - 1 - i;
        swap = bytes[i];
        bytes[i] = bytes[j];
        bytes[j] = swap;
    }
}

static void validate_magic_number (FILE * fp) {
    // validate that first two bytes are 0
    bool a = read_uint8(fp, 0) != 0;
    bool b = read_uint8(fp, 1) != 0;
    if (a || b) {
        fprintf(stderr, "Found unexpected value in first two bytes, aborting.\n");
        exit(EXIT_FAILURE);
    }
}
