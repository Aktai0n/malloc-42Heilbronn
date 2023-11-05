


#include <sys/mman.h>
#include <stddef.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#include "ft_malloc.h"
#include "memory_page/memory_page.h"

void free(void* ptr) {
    if (ptr == NULL) {
        return;
    }
    
    if (!destroy_memory_page(ptr)) {
        fprintf(stderr, "munmap failed: %s\n", strerror(errno));
        exit(1);
    }
}

