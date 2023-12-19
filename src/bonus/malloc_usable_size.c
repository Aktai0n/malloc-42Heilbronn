
#include <errno.h>

#include "../memory/memory.h"


size_t malloc_usable_size(void* ptr) {
    if (ptr == NULL) {
        errno = EINVAL;
        return 0;
    }
    return available_memory_size(ptr);
}