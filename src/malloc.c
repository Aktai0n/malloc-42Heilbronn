
#include <stddef.h>
#include <sys/mman.h>
#include <errno.h>
#include <pthread.h>

#include "ft_malloc.h"

// initialize a static mutex
pthread_mutex_t g_alloc_mutex = PTHREAD_MUTEX_INITIALIZER;
struct s_heap g_heap = { 0 };

void* malloc(size_t size) {
    void* ptr = MAP_FAILED;
    if (ptr == MAP_FAILED) {
        errno = ENOMEM;
        return NULL;
    }
    *(size_t*)ptr = size;
    ptr = (size_t*)ptr + 1;
    return ptr;
}