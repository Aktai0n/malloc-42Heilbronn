
#include <stddef.h>
#include <sys/mman.h>
#include <errno.h>
#include <pthread.h>

#include "ft_malloc.h"
#include "memory_page/memory_page.h"
#include "defines.h"

// initialize a static mutex
pthread_mutex_t g_alloc_mutex = PTHREAD_MUTEX_INITIALIZER;
struct s_heap g_heap = { 0 };

void* malloc(size_t size) {
    size = ALIGN_ALLOC_SIZE(size);
    void* ptr = init_memory_page(size, TINY_PAGE, 0);
    if (ptr == NULL) {
        return NULL;
    }
    return (void*)((size_t)ptr + sizeof(t_memory_page));
}