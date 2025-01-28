# Developer notes

## CMake

The project has been initialized with a [CMakeLists.txt](CMakeLists.txt)-based
configuration for building with CMake:

```console
# change into the build directory
$ cd build/

# generate the build files
$ cmake ..

# build the project
$ cmake --build .

# install the project to <repo>/build/dist
$ cmake --install .

# run the program to see if it works
$ ./dist/bin/example-idx-any-type -h
```

Should output something like:

```console
Usage: ./example-idx-any-type FILEPATH
   Illustrate the usage of the idx library by reading IDX-formatted
   data from a binary file located at FILEPATH.
```

The [`data`](data) directory contains a few test files in IDX formats. Each file's body uses a
different data type for its values.

```console
$ ./dist/bin/example-idx-any-type ../data/float.idx
header: {
    type: 0x0d (float),
    ndims: 2,
    lengths: [3, 2],
    nelems: 6,
    bodystart: 12
}
body: [
    -3.40282e+38
    -1
    -0
     0
     1
     3.40282e+38
]
```

## Testing

Building and running the tests requires that [Criterion](https://github.com/Snaipe/Criterion) is
installed on the system, e.g. with

```console
$ sudo apt install libcriterion-dev
```

Run the tests with

```console
$ ./dist/bin/test_idx -j1 --verbose
```

 or simply, with:

```console
$ ctest
```

The CMake variable `IDX_BUILD_TESTING` can be used to build the
tests.

- When this project is the top project, `IDX_BUILD_TESTING` inherits the value of
  CTest's `BUILD_TESTING`, which is set to `ON` by default.
- When this project is not the top project but instead it is used as a dependency to a parent
  project, the default is to not build the tests. However, building the tests is still possible by
  setting the `IDX_BUILD_TESTING` to `ON`, e.g like so:

```console
$ cmake -DIDX_BUILD_TESTING=ON ..
```

## `clang-format`

The file `.clang-format` contains an initial configuration for (automatic) formatting with [clang-format](https://clang.llvm.org/docs/ClangFormat.html). Run the formatter with e.g.:

```console
# dry run on main.c
$ clang-format -Werror --dry-run main.c

# format in place all *.c and *.h files under ./src
$ clang-format -i `find ./src -type f -name '*.[c|h]'`
```
