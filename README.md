# malloc-42Heilbronn
A re-implementation of the *alloc functions using mmap

1. [Usage](#usage)
2. [Implementation](#implementation)
3. [Resources](#resources)


## Usage:
First, make sure you have `make` and either `gcc` or `clang` installed.

### Clone the repository:
```bash
git clone https://github.com/Aktai0n/malloc-42Heilbronn.git
```

### Build and run the project:
```bash
make
```
This will build a shared library with a platform dependent name and a symlink to it called `libft_malloc.so`.<br>
The `run.sh` script can be used to load the library into other executables as follows:
```bash
./run.sh /path/to/the/executable [flags-for-the-executable]
```
### Build and run tests:
```bash
make test
```
This will build the shared library, symlink and an executable called `tester`<br>
Use
```bash
./tester
```
to run all tests.<br>
_**Note**: Make sure to run `make fclean` before building the tests if the library was built normally before. This needs to be done because `make test` exposes all symbols of the library_<br>
Or use
```bash
./tester [test-types]
```
to only run specific tests. _The types of tests can be listed by using `./tester help`_


## Implementation:



## Resources:
- [Glibc's malloc documentation](https://sourceware.org/glibc/wiki/MallocInternals)
- [Glibc's malloc implementation](https://sourceware.org/git/?p=glibc.git;a=blob;f=malloc/malloc.c;h=948f9759afa16f74b7bf3848589c0766e9f42cfb;hb=HEAD)
- [Wikibooks on malloc](https://en.wikibooks.org/wiki/C_Programming/stdlib.h/malloc)

- [Working with shared libs](https://amir.rachum.com/shared-libraries/)
- [Options for working with shared libs like -fPIE, -Wl, -static](https://www.man7.org/linux/man-pages/man1/gcc.1.html)
- [Visibility in C and C++](https://gcc.gnu.org/wiki/Visibility)
- [Constructor and Destructor in C](https://stackoverflow.com/questions/2053029/how-exactly-does-attribute-constructor-work)
