#ifndef MALLOC_H
#define MALLOC_H


#include <sys/mman.h>
#include <stdint.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>


// typedef union u_alloc {
//     union {
//         uint64_t size;
//         void *next;
//     } data;

//     char buf[8];
// } t_alloc;

typedef struct s_alloc {
    union {
        struct s_alloc* prev;
        size_t prev_size;
    };

    union {
        struct s_alloc* next;
        size_t size;
    };
} t_alloc;

enum e_memory_page {
    TINY = 1,
    SMALL = 2,
    LARGE = 3
};

typedef struct s_memory_page {
    t_alloc* allocated_list;
    t_alloc* free_list;
    size_t size;
    enum e_memory_page type;
    struct s_memory_page* next;
} t_memory_page;

struct s_heap {
    t_memory_page* small_pages;
    t_memory_page* medium_pages;
    t_memory_page* large_pages;
};

// both are defined in malloc.c
extern struct s_heap g_heap;
extern pthread_mutex_t g_alloc_mutex;


#define IS_ALLOCATED_FLAG ((size_t)0b1)
#define ALLOC_FLAGS (IS_ALLOCATED_FLAG)

// #define GET_ALLOC_SIZE(alloc_node) (uint64_t)(alloc_node)

// returns the true size of an allocated block of memory by
// masking away the two lowest bits 
inline size_t get_alloc_size(const t_alloc* alloc_node) {
    return alloc_node->size & ~ALLOC_FLAGS;
}

// determines whether the block is currently in use or not
inline bool is_allocated(const t_alloc* alloc_node) {
    return alloc_node->size & IS_ALLOCATED_FLAG;
}

inline t_alloc* get_next_alloc(const t_alloc* current_node) {
    return (t_alloc*)(current_node->size & ~ALLOC_FLAGS);
}

inline t_alloc* get_prev_alloc(const t_alloc* current_node) {
    (void)current_node;
}


void* malloc(size_t size);
void* calloc(size_t n, size_t size);
void* realloc(void* ptr, size_t size);
void free(void* ptr);

void show_alloc_mem(void);


#endif // MALLOC_H