
#include <stddef.h>
#include <sys/mman.h>
#include <errno.h>


#include "ft_malloc.h"
#include "defines.h"
#include "memory/memory.h"

void* malloc(size_t size) {
    if (size == 0) {
        return NULL;
    }
    void* ptr = allocate_memory(size, false);
    return ptr;
}
