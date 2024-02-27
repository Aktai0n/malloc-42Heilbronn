
#include "ft_malloc.h"
#include "defines.h"
#include "memory/memory.h"

void* calloc(size_t n, size_t size) {
    if (n == 0 || size == 0) {
        return NULL;
    }
    return allocate_memory(n * size, true);
}
