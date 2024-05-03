#pragma once

#include <stddef.h>
#include <stdbool.h>

#define IS_ALLOCATED_FLAG ((size_t)0b1)
#define IS_LAST_BLOCK_FLAG ((size_t)0b10)
#define ALLOC_FLAGS (IS_ALLOCATED_FLAG | IS_LAST_BLOCK_FLAG)

typedef struct s_block_base {
    size_t size;
} t_block_base;

/// @brief Returns the true size of a block of memory by
///        masking away the allocation flags
/// @param block The block whose size is returned
/// @return The usable size of the block
inline size_t get_block_size(t_block_base block) {
    return block.size & ~ALLOC_FLAGS;
}

/// @brief determines whether the block is currently in use or not
inline bool is_allocated(t_block_base block) {
    return block.size & IS_ALLOCATED_FLAG;
}

/// @brief determines whether this block is the first or last 
///        in memory on the current memory page
inline bool is_last_block(t_block_base block) {
    return block.size & IS_LAST_BLOCK_FLAG;
}


/// @brief Sets or resets a flag in the specified memory block
/// @param block The memory block to set the flag in
/// @param flag The flag that should be modified
/// @param state The state to which the flag will be set to
inline void set_block_flag(
    t_block_base* block,
    size_t flag,
    bool state
) {
    if (state == true) {
        block->size = block->size | flag;
    } else {
        block->size = block->size & ~flag;
    }
}

/// @brief sets the size of a block of memory without affecting
///        the allocation flags
/// @param block The block whose size should be changed
/// @param size The new usable size of the block
inline void set_block_size(t_block_base* block, const size_t size) {
    block->size = (block->size & ALLOC_FLAGS) | (size & ~ALLOC_FLAGS);
}
