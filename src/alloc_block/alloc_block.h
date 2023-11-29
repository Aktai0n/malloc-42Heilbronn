#pragma once

#include <stddef.h>
#include <stdbool.h>

typedef struct s_alloc_block {
    struct s_alloc_block* next;
    size_t size;
} t_alloc_block;

#define IS_ALLOCATED_FLAG ((size_t)0b1)
#define IS_LAST_BLOCK_FLAG ((size_t)0b10)
#define ALLOC_FLAGS (IS_ALLOCATED_FLAG | IS_LAST_BLOCK_FLAG)

// returns the true size of a block of memory by
// masking away the allocation flags
inline size_t get_alloc_size(const t_alloc_block* block) {
    return block->size & ~ALLOC_FLAGS;
}

// sets the size of a block of memory while not affecting
// the allocation flags
inline void set_alloc_size(t_alloc_block* block, const size_t size) {
    block->size = size & ~ALLOC_FLAGS;
}

// determines whether the block is currently in use or not
inline bool is_allocated(const t_alloc_block* block) {
    return block->size & IS_ALLOCATED_FLAG;
}

// determines whether the block is the last block of memory
// on this memory page
inline bool is_last_block(const t_alloc_block* block) {
    return block->size & IS_LAST_BLOCK_FLAG;
}

/// @brief sets or resets a flag in the specified memory block
/// @param block the memory block to set the flag in
/// @param flag the flag that should be modified
/// @param state the state to which the flag will be set to
inline void set_alloc_block_flag(t_alloc_block* block, size_t flag, bool state) {
    if (state == true) {
        block->size = block->size | flag;
    } else {
        block->size = block->size & ~flag;
    }
}

inline t_alloc_block* get_next_block_in_memory(t_alloc_block* block) {
    if (is_last_block(block)) {
        return NULL;
    }
    return (t_alloc_block*)((size_t)block + sizeof(*block) + get_alloc_size(block));
}



void add_to_alloc_list(t_alloc_block** list, t_alloc_block* new_block);
t_alloc_block* delete_from_alloc_list(t_alloc_block** list, t_alloc_block* to_remove);
// finds a free block of memory to allocate using a best fit approach
t_alloc_block* find_alloc_block(t_alloc_block* list_head, const size_t size);


bool merge_alloc_block(t_alloc_block* block, t_alloc_block** free_list);

t_alloc_block* split_alloc_block(
    t_alloc_block* block,
    size_t split_size,
    t_alloc_block** free_list,
    t_alloc_block** allocated_list
);

t_alloc_block* init_alloc_block(
    const size_t size,
    t_alloc_block** free_list,
    t_alloc_block** allocated_list
);