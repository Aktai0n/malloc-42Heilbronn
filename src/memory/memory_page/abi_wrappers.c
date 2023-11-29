
#include <sys/mman.h>
#include <stddef.h>
#include <stdbool.h>

#include "memory_page.h"

void* call_mmap(size_t size, int additional_flags) {
    void* ptr = mmap(
        NULL,
        size,
        PROT_READ | PROT_WRITE,
        additional_flags | MAP_PRIVATE | MAP_ANONYMOUS,
        -1,
        0
    );
    if (ptr == MAP_FAILED) {
        return NULL;
    }
    return ptr;
}

bool call_munmap(t_memory_page* page) {
    size_t size = page->size + sizeof(*page);
    if (munmap((void*)page, size) == -1) {
        return false;
    }
    return true;
}

