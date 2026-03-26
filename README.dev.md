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

```console
$ ./dist/bin/example-uint8-1d --help
Usage: ./dist/bin/example-uint8-1d FILEPATH

   Illustrate the usage of the idx library by reading IDX-formatted data
   from a binary file located at FILEPATH. The 3rd byte in the file should
   be 0x08 to indicate that the body consists of 8-bit unsigned integer values,
   and the 4th byte should be 1 to indicate that there is just 1 dimension.

   The program reads the data from file and prints the result to standard
   output.

$ ./dist/bin/example-uint8-1d ../data/uint8.1d.idx
  0   1  15  16 127 255
```

```console
$ ./dist/bin/example-int16-2d --help
Usage: ./dist/bin/example-int16-2d FILEPATH

   Illustrate the usage of the idx library by reading IDX-formatted data
   from a binary file located at FILEPATH. The 3rd byte in the file should
   be 0x0B to indicate that the body consists of 16-bit signed integer values,
   and the 4th byte should be 2 to indicate that there are 2 dimensions.

   The program reads the data from file and prints the result to standard
   output.

$ ./dist/bin/example-int16-2d ../data/int16.2d.idx
-32768     -1
     0      0
     1  32767
```

```console
$ ./dist/bin/idxread -h
Usage: ./dist/bin/idxread FILEPATH

    Read IDX-formatted data from a binary file located at FILEPATH.

$ ./dist/bin/idxread ../data/float.2d.idx
-3.403e+38         -1
        -0          0
         1  3.403e+38
```

## Build flags

| CMake flag                   | Description                                          |
| ---------------------------- | ---------------------------------------------------- |
| `EXAMPLE_IDXREAD_WITH_ASAN`  | Address sanitizing for executable `idxread`          |
| `EXAMPLE_INT16_2D_WITH_ASAN` | Address sanitizing for executable `example-int16-2d` |
| `EXAMPLE_UINT8_1D_WITH_ASAN` | Address sanitizing for executable `example-uint8-1d` |
| `IDX_BUILD_EXAMPLES`         | Building the libidx examples                         |
| `IDX_BUILD_IDXREAD`          | Building the `idxread` executable                    |
| `IDX_WITH_ASAN`              | Address sanitizing for the idx library               |

To enable/disable a flag, configure the build via `ccmake ..`, or via a command line argument
with:

```console
# e.g., for IDX_BUILD_EXAMPLES
$ cmake -DIDX_BUILD_EXAMPLES=OFF ..
```

## Address sanitizing

To use address sanitizing, you may need to install an extra dependency, e.g. like so:

```console
# (Ubuntu)
sudo apt install libasan8
```

## `clang-format`

The file `.clang-format` contains an initial configuration for (automatic) formatting with [clang-format](https://clang.llvm.org/docs/ClangFormat.html). Run the formatter with e.g.:

```console
# dry run on main.c
$ clang-format -Werror --dry-run main.c

# format in place all *.c and *.h files under ./src
$ clang-format -i `find ./src -type f -name '*.[c|h]'`
```
