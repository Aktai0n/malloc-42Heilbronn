
#include <stddef.h>
#include <sys/mman.h>
#include <errno.h>
#include <pthread.h>

#include "malloc.h"

// initialize a static mutex
pthread_mutex_t g_alloc_mutex = PTHREAD_MUTEX_INITIALIZER;
struct s_heap g_heap = { 0 };

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