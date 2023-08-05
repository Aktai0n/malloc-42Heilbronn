


#include <sys/mman.h>
#include <stddef.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

void free(void* ptr) {
    if (ptr == NULL) {
        return;
    }

    ptr = (size_t*)ptr - 1;
    size_t size = *(size_t*)ptr;
    int result = munmap(ptr, size);
    if (result == -1) {
        fprintf(stderr, "mmap failed: %s\n", strerror(errno));
        exit(1);
    }
}

