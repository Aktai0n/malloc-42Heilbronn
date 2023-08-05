#ifndef MALLOC_H
#define MALLOC_H


#include <sys/mman.h>
#include <stdint.h>
#include <unistd.h>
#include <stdbool.h>


// typedef union u_alloc {
//     union {
//         uint64_t size;
//         void *next;
//     } data;

//     char buf[8];
// } t_alloc;

typedef struct  {
    union {
        void* prev;
        size_t prev_size;
    };

    union {
        void* next;
        size_t size;
    };
} t_alloc;

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
    
}

void* malloc(size_t size);
void free(void* ptr);


#endif // MALLOC_H