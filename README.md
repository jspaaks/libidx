# libidx

For instructions on building etc, see [`README.dev.md`](README.dev.md).

C library that can read IDX files, such as those from Yann LeCun et al's handwritten
numbers data set as well as the Zalando fashion data set.

The IDX format is described here: https://www.fon.hum.uva.nl/praat/manual/IDX_file_format.html

## Libraries

- `libidx`: C library to read IDX files. For library API, see
    [`include/idx/idx.h`](include/idx/idx.h).

## Executables

- `example-idx-any-type`: example program illustrating how to read data of any of the IDX-supported
    types (`uint8_t` / `unsigned char`, `int8_t` / signed `char`, `int16_t` / signed `short` ,
    `int32_t` / signed `int`, 32-bit `float`, 64-bit `double`).
- `example-idx-uint8`: example program illustrating how to read data that is known to be in
    `uint8_t` / `unsigned char` type.

## Alternatives

- https://github.com/JohnnyVM/idxc
- https://github.com/BinAl-Sadiq/.idx

## Acknowledgements

_This project was initialized using [Copier](https://pypi.org/project/copier) and the [copier-template-for-c-projects](https://github.com/jspaaks/copier-template-for-c-projects)._

[![Copier](https://img.shields.io/endpoint?url=https://raw.githubusercontent.com/copier-org/copier/master/img/badge/badge-grayscale-inverted-border-orange.json)](https://github.com/copier-org/copier)
