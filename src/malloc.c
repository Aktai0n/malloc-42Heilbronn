
#include <stddef.h>
#include <sys/mman.h>
#include <errno.h>
#include <pthread.h>

#include "ft_malloc.h"
#include "defines.h"
#include "memory/memory.h"

// initialize a static mutex
pthread_mutex_t g_alloc_mutex = PTHREAD_MUTEX_INITIALIZER;
struct s_heap g_heap = { 0 };

void* malloc(size_t size) {
    void* ptr = allocate_memory(size);
    return ptr;
}