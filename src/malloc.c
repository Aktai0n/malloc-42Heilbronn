
#include <stddef.h>
#include <sys/mman.h>
#include <errno.h>


void* malloc(size_t size) {
    void* ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);

    if (ptr == MAP_FAILED) {
        errno = ENOMEM;
        return NULL;
    }
    *(size_t*)ptr = size;
    ptr = (size_t*)ptr + 1;
    return ptr;
}