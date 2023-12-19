#include "ft_malloc.h"
#include "defines.h"
#include "memory/memory.h"


void* realloc(void* ptr, size_t size) {
    if (ptr == NULL) {
        return malloc(size);
    } else if (size == 0) {
        free(ptr);
        return NULL;
    }
    return reallocate_memory(ptr, size);
}
