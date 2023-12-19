
#include "ft_malloc.h"
#include "defines.h"
#include "memory/alloc_block/alloc_block.h"

/// @brief Special implementation for bzero since FT_MALLOC_ALIGNMENT
///        is guranteed to be a multiple of 8
static void bzero_calloc_internal_(t_alloc_block* block) {
    size_t* start = (size_t*)((size_t)block + sizeof(*block));
    size_t* end = (size_t*)((size_t)start + get_alloc_size(block));
    while (start < end) {
        *start = 0;
        ++start;
    }
}

void* calloc(size_t n, size_t size) {
    return malloc(n * size);
}