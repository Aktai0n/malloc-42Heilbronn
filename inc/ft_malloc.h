#ifndef FT_MALLOC_H
#define FT_MALLOC_H


#include <sys/mman.h>
#include <stdint.h>
#include <unistd.h>
#include <pthread.h>


typedef struct s_memory_page t_memory_page;

struct s_heap {
    t_memory_page* tiny_pages;
    t_memory_page* small_pages;
    t_memory_page* large_pages;
};

// both are defined in malloc.c
extern struct s_heap g_heap;
extern pthread_mutex_t g_alloc_mutex;




// #define GET_ALLOC_SIZE(alloc_node) (uint64_t)(alloc_node)




void* malloc(size_t size);
void* calloc(size_t n, size_t size);
void* realloc(void* ptr, size_t size);
void free(void* ptr);

void show_alloc_mem(void);


#endif // FT_MALLOC_H
