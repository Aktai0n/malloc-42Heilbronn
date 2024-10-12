
#include "ft_malloc_internal.h"
#include "defines.h"
#include "memory/memory.h"

void* calloc(size_t n, size_t size) {
    if (n == 0 || size == 0) {
        return NULL;
    }
    FT_MALLOC_ACQUIRE_LOCK(&g_alloc_mutex);
    void* ptr = allocate_memory(n * size, true);
    FT_MALLOC_RELEASE_LOCK(&g_alloc_mutex);
    return ptr;
}
