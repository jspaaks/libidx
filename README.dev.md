# Developer notes


## Data

The [`data`](data) directory contains a few test files in IDX format. Each file's body uses a
different data type for its values.

## Building

The project has been initialized with a [CMakeLists.txt](CMakeLists.txt)-based
configuration for building with CMake:

```console
# change into the build directory
$ cd build/

# generate the build files
$ cmake -DCMAKE_BUILD_TYPE=Debug ..

# build the project
$ cmake --build .

# install the project to <repo>/build/dist
$ cmake --install . --prefix dist/
```

The above should produce a library, libidx, as well as some executables. 

```text
$ ./dist/bin/example-idx-uint8
Usage: ./example-idx-uint8 FILEPATH
   Illustrate the usage of the idx library by reading IDX-formatted data
   from a binary file located at FILEPATH. The 3rd byte in the file should
   be 0x08 to indicate that the body consists of uint8 values.
$ ./dist/bin/example-idx-uint8 ../data/uint8.idx
header: {
    type: 0x08 (uint8),
    ndims: 2,
    lengths: [3, 2],
    nelems: 6,
    bodystart: 12
}
body: [
    0
    1
    15
    16
    127
    255
]
$ ./dist/bin/example-idx-uint8 ../data/int8.idx
Expected data in file "../data/int8.idx" to be of type 0x08 (uint8) but found 0x09 (int8), aborting.
```

```console
$ ./dist/bin/example-idx-any-type
Usage: ./example-idx-any-type FILEPATH
   Illustrate the usage of the idx library by reading IDX-formatted
   data from a binary file located at FILEPATH.
$ ./dist/bin/example-idx-any-type ../data/uint8.idx
header: {
    type: 0x08 (uint8),
    ndims: 2,
    lengths: [3, 2],
    nelems: 6,
    bodystart: 12
}
body: [
    0
    1
    15
    16
    127
    255
]
```

## Testing

Building and running the tests requires that [Criterion](https://github.com/Snaipe/Criterion) is
installed on the system, e.g. with

```console
$ sudo apt install libcriterion3 libcriterion-dev
```

The CMake variable `IDX_BUILD_TESTING` can be used to control whether to build the
tests.

- When this project is the top project, `IDX_BUILD_TESTING` inherits the value of
  CTest's `BUILD_TESTING`, which is set to ON by default.
- When this project is not the top project but instead it is used as a dependency to a parent
  project, the default is to not build the tests. However, building the tests is still possible by
  setting the `IDX_BUILD_TESTING` to `ON`, e.g like so:

```console
$ cmake -DIDX_BUILD_TESTING=ON ..
```

Afterwards, run the tests with

```console
$ ./dist/bin/test_idx -j1 --verbose
```

## Address sanitizing

To use address sanitizing, you may need to install an extra dependency, e.g. like so:

```console
# (Ubuntu)
sudo apt install libasan8
```

The CMake variable `EXAMPLE_IDX_UINT8_WITH_ASAN` can be used to enable address sanitizing on the
executable `example-idx-uint8`. `EXAMPLE_IDX_UINT8_WITH_ASAN`'s value is `OFF` by default. To
enable it, configure the build via `ccmake ..`, or via a command line argument with:

```console
$ cmake -DEXAMPLE_IDX_UINT8_WITH_ASAN=ON ..
```

The CMake variable `EXAMPLE_IDX_ANY_TYPE_WITH_ASAN` can be used to enable address sanitizing on the
executable `example-idx-any-type`. `EXAMPLE_IDX_ANY_TYPE_WITH_ASAN`'s value is `OFF` by default. To
enable it, configure the build via `ccmake ..`, or via a command line argument with:

```console
$ cmake -DEXAMPLE_IDX_ANY_TYPE_WITH_ASAN=ON ..
```

The CMake variable `IDX_WITH_ASAN` can be used to enable address sanitizing on the
library `libidx.so`. `IDX_WITH_ASAN`'s value is `OFF` by default. To
enable it, configure the build via `ccmake ..`, or via a command line argument with:

```console
$ cmake -DIDX_WITH_ASAN=ON ..
```

## `clang-format`

The file `.clang-format` contains an initial configuration for (automatic) formatting with [clang-format](https://clang.llvm.org/docs/ClangFormat.html). Run the formatter with e.g.:

```console
# dry run on main.c
$ clang-format -Werror --dry-run main.c

# format in place all *.c and *.h files under ./src
$ clang-format -i `find ./src -type f -name '*.[c|h]'`
```
