#include <errno.h>
#include <string.h>
#include <unistd.h>

#include "ft_malloc_internal.h"
#include "libft.h"
#include "memory/memory.h"

void free(void* ptr) {
    if (ptr == NULL) {
        return;
    }

    FT_MALLOC_ACQUIRE_LOCK(&g_alloc_mutex);
    // any replacement free() is required to preserve errno
    int save_errno = errno;
    if (!release_memory(ptr)) {
        // ft_dprintf(STDERR_FILENO, "free failed: %s %p\n", strerror(errno), ptr);
        // show_alloc_mem();
    }
    errno = save_errno;
    FT_MALLOC_RELEASE_LOCK(&g_alloc_mutex);
}
