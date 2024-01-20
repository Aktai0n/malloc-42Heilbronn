
#include "ft_malloc.h"
#include "defines.h"
#include "memory/alloc_block/alloc_block.h"



void* calloc(size_t n, size_t size) {
    return malloc(n * size);
}