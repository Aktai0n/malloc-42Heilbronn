#pragma once

#include "../../block_base/block_base.h"

typedef struct s_small_block {
    t_block_base curr;
    t_block_base prev;
} t_small_block;

// ---------------------- inline functions -----------------------

/// @brief returns the small_block structure associated to
///        an allocated block of memory
/// @param ptr A pointer pointing to the usable memory in
///            the small_block structure
/// @return The structure associated to the memory
inline t_small_block* get_small_block(void* ptr) {
    return (t_small_block*)((size_t)ptr - sizeof(t_small_block));
}

/// @brief Returns the data pointer assoiciated to a
///        small block of memory
/// @param block The memory block whose data pointer should
///              be retrieved
/// @return A pointer to the usable data region
///         in the memory block
inline void* get_small_block_data(t_small_block* block) {
    return (void*)((size_t)block + sizeof(*block));
}

/// @brief Returns the next block in memory on the current memory page
/// @param block A pointer to the current block of memory
/// @return The next block in memory or
///         NULL if the current block is the last block on the
///         current memory page
inline t_small_block* get_next_small_block(t_small_block* block) {
    if (is_last_block(block->curr)) {
        return NULL;
    }
    return (t_small_block*)(
        (size_t)block + sizeof(*block) + get_block_size(block->curr)
    );
}

/// @brief Returns the previous block in memory on the current memory page
/// @param block A pointer to the current block of memory
/// @return The previous block in memory or
///         NULL if the current block is the first block on the
///         current memory page
inline t_small_block* get_prev_small_block(t_small_block* block) {
    size_t prev_size = get_block_size(block->prev);
    if (prev_size == 0) {
        return NULL;
    }
    return (t_small_block*)(
        (size_t)block - (sizeof(*block) + prev_size)
    );
}

// ---------------------- small_block_list.c -----------------------

/// @brief Finds a free block of memory to allocate
///        using a first fit approach
/// @param list The list of blocks to search in
/// @param size The smallest size the block should have
/// @return A memory block that has at least size bytes or
///         NULL if no block is found in the list
t_small_block* find_small_block(
    t_small_block* list,
    const size_t size
);



bool merge_small_block(t_small_block** block);

bool split_small_block(t_small_block* block, const size_t split_size);