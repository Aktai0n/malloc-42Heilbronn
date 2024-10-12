
#include <stddef.h>
#include <sys/mman.h>
#include <errno.h>


#include "ft_malloc_internal.h"
#include "defines.h"
#include "memory/memory.h"

void* malloc(size_t size) {
    if (size == 0) {
        return NULL;
    }
    FT_MALLOC_ACQUIRE_LOCK(&g_alloc_mutex);
    void* ptr = allocate_memory(size, false);
    FT_MALLOC_RELEASE_LOCK(&g_alloc_mutex);
    return ptr;
}
