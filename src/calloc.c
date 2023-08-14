#include "ft_malloc.h"


void* calloc(size_t n, size_t size) {
    return malloc(n * size);
}