
#include <errno.h>

#include "../memory/memory.h"
#include "ft_malloc_internal.h"

size_t malloc_usable_size(void* ptr) {
    if (ptr == NULL) {
        errno = EINVAL;
        return 0;
    }
    FT_MALLOC_ACQUIRE_LOCK(&g_alloc_mutex);
    size_t size = available_memory_size(ptr, &g_heap);
    FT_MALLOC_RELEASE_LOCK(&g_alloc_mutex);
    return size;
}
