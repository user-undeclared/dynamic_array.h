# dynamic\_array.h

a dynamic array implementation in ANSI C, in my preferred coding style, and in
the form of a single header-only library. this implementation puts the dynamic
array's metadata (length, capacity, etc.) before its data so that the entire
dynamic array fits in just one allocation

## usage

this library is entirely contained in [`dynamic_array.h`](./dynamic_array.h),
which is a small and (hopefully) self-documenting source file, so all that you
should need to do to figure out how to use this is to take a look at the file.
i know you can do it :D

## examples

this code repository also contains several self-contained examples in the
[`examples/`](./examples/) directory that show more general use cases for these
dynamic arrays, as well as how each of the library's procedures fit together and
are designed to be used

the examples are built with the included [`makefile`](./makefile). to build all
of them, simply run
[make](https://pubs.opengroup.org/onlinepubs/9699919799/utilities/make.html):

```console
$ make
```
