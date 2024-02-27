


#include <sys/mman.h>
#include <stddef.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#include "ft_malloc.h"
#include "memory/memory.h"
#include "libft.h"

void free(void* ptr) {
    if (ptr == NULL) {
        return;
    }

    // any replacement free() is required to preserve errno
    int save_errno = errno;

    if (!release_memory(ptr)) {
        ft_dprintf(STDERR_FILENO, "free failed: %s\n", strerror(errno));
    }
    
    // if (!destroy_memory_page(ptr)) {
    //     fprintf(stderr, "munmap failed: %s\n", strerror(errno));
    //     exit(1);
    // }
    errno = save_errno;
}

