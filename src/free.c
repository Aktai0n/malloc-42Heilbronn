


#include <sys/mman.h>
#include <stddef.h>


void free(void* ptr) {
    if (ptr == NULL) {
        return;
    }

    ptr -= sizeof(size_t);
    size_t size = *(size_t*)ptr;
    
}