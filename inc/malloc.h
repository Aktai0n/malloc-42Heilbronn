#ifndef MALLOC_H
#define MALLOC_H


#include <sys/mman.h>
#include <stdint.h>
#include <unistd.h>


typedef union u_alloc {
    union {
        uint64_t size;
        void *next;
    } data;

    char [8];
} t_alloc;

// #define GET_ALLOC_SIZE(alloc_node) (uint64_t)(alloc_node)

// returns the true size of an allocated block of memory by
// masking away the two lowest bits 
inline uint64_t get_alloc_size(const t_alloc* alloc_node) {
    return alloc_node->data.size & ~0b11;
}

// determines whether the block is currently in use or not
inline bool is_allocated(const t_alloc* alloc_node) {
    return alloc_node->data.size & 0b1;
}


#endif // MALLOC_H