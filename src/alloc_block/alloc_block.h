#pragma once

#include <stddef.h>
#include <stdbool.h>

typedef struct s_alloc_block {
    struct s_alloc_block* next;
    struct s_alloc_block* prev;
    size_t size;
} t_alloc_block;

#define IS_ALLOCATED_FLAG ((size_t)0b1)
#define IS_LAST_BLOCK_FLAG ((size_t)0b10)
#define ALLOC_FLAGS (IS_ALLOCATED_FLAG | IS_LAST_BLOCK_FLAG)

// returns the true size of an allocated block of memory by
// masking away the two lowest bits 
inline size_t get_alloc_size(const t_alloc_block* alloc_node) {
    return alloc_node->size & ~ALLOC_FLAGS;
}

// determines whether the block is currently in use or not
inline bool is_allocated(const t_alloc_block* alloc_node) {
    return alloc_node->size & IS_ALLOCATED_FLAG;
}

inline void set_alloc_flag(t_alloc_block* block, bool flag) {
    if (flag == true) {
        block->size = block->size | IS_ALLOCATED_FLAG;
    } else {
        block->size = block->size & ~IS_ALLOCATED_FLAG;
    }
}

inline t_alloc_block* get_next_alloc(const t_alloc_block* current_node) {
    return current_node->next;
}

inline t_alloc_block* get_prev_alloc(const t_alloc_block* current_node) {
    return current_node->prev;
}

typedef struct s_tiny_block {
    size_t size;
} t_tiny_block;

inline size_t get_tiny_block_size(const t_tiny_block* block) {
    return block->size & ~ALLOC_FLAGS;
}

inline t_tiny_block* get_next_tiny_block(const t_tiny_block* block) {
    if ((block->size & IS_LAST_BLOCK_FLAG) != 0) {
        return NULL;
    }
    return (t_tiny_block*)(block->size & ~ALLOC_FLAGS);
}

