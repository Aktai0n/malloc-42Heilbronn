#include "ft_malloc_internal.h"
#include "defines.h"
#include "memory/memory.h"

void* realloc(void* ptr, size_t size) {
    if (ptr == NULL) {
        return malloc(size);
    } else if (size == 0) {
        free(ptr);
        return NULL;
    }
    FT_MALLOC_ACQUIRE_LOCK(&g_alloc_mutex);
    ptr = reallocate_memory(ptr, size);
    FT_MALLOC_RELEASE_LOCK(&g_alloc_mutex);
    return ptr;
}
