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

## Address sanitizing

To use address sanitizing, you may need to install an extra dependency, e.g. like so:

```console
# (Ubuntu)
sudo apt install libasan8
```

The CMake variable `EXAMPLE_INT16_2D_WITH_ASAN` can be used to enable address sanitizing on the
executable `example-int16-2d`. `EXAMPLE_INT16_2D_WITH_ASAN`'s value is `OFF` by default. To
enable it, configure the build via `ccmake ..`, or via a command line argument with:

```console
$ cmake -DEXAMPLE_INT16_2D_WITH_ASAN=ON ..
```

The CMake variable `EXAMPLE_UINT8_1D_WITH_ASAN` can be used to enable address sanitizing on the
executable `example-uint8-1d`. `EXAMPLE_UINT8_1D_WITH_ASAN`'s value is `OFF` by default. To
enable it, configure the build via `ccmake ..`, or via a command line argument with:

```console
$ cmake -DEXAMPLE_UINT8_1D_WITH_ASAN=ON ..
```

The CMake variable `EXAMPLE_IDXREAD_WITH_ASAN` can be used to enable address sanitizing on the
executable `idxread`. `EXAMPLE_IDXREAD_WITH_ASAN`'s value is `OFF` by default. To
enable it, configure the build via `ccmake ..`, or via a command line argument with:

```console
$ cmake -DEXAMPLE_IDXREAD_WITH_ASAN=ON ..
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
