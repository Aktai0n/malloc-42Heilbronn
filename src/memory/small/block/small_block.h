#pragma once

#include <stddef.h>
#include <stdint.h>

#include "small_block_struct.h"
#include "utils.h"

// ---------------------- inline functions -----------------------

/// @brief returns the small block structure associated to
///        an allocated block of memory
/// @param ptr A pointer pointing to the usable memory in
///            the small block structure
/// @return The structure associated to the memory
inline t_small_block* get_small_block(void* ptr) {
    return (t_small_block*)((uintptr_t)ptr - sizeof(t_small_block));
}

/// @brief Returns the data pointer assoiciated to a
///        small block of memory
/// @param block The memory block whose data pointer should
///              be retrieved
/// @return A pointer to the usable data region
///         in the memory block
inline void* get_small_block_data(t_small_block* block) {
    return (void*)((uintptr_t)block + sizeof(*block));
}

/// @brief Returns a pointer pointing to the address just behind
///        the last data byte in the given block. 
///        Essentially returning an `end iterator`
/// @param block The block whose end should be calculated
/// @return A pointer pointing behind the data region in the
///         memory block
/// @attention Dereferencing this pointer is undefined behaviour
inline void* get_small_block_data_end(t_small_block* block) {
    return (void*)(
        (uintptr_t)get_small_block_data(block) +
        get_block_size(block->curr)
    );
}

inline void copy_small_block_data(
    t_small_block* src,
    t_small_block* dst
) {
    ft_malloc_memcpy(
        get_small_block_data(src),
        get_small_block_data(dst),
        get_block_size(src->curr)
    );
}

// ---------------------- small_block_operations.c ------------------

/// @brief Searches for an available block of memory with at least
///        `size` bytes of capacity in the given list of blocks
///        and marks it as in use
/// @param size The minimal available bytes the block should have
/// @param list The list of memory blocks on this memory page
/// @return The reserved block of memory or
///         NULL if no fitting block was found
t_small_block* allocate_small_block(
    const size_t size,
    t_small_block* list
);

/// @brief Resets the block of memory as available and defragments
///        the memory if possible.
///        If defragmentation is performed, the address of `block`
///        might change
/// @param block The memory block to be reclaimed
void deallocate_small_block(t_small_block** block);

/// @brief Attempts to increase the size of the given
///        block of memory by:
///        1. Checking whether it has enough space available.
///        2. Trying to merge it with its next block in memory
///        3. Reserving a new block on the current memory page
///           and copying the contents over
/// @param block The memory block whose size should be increased
/// @param size The minimal number of bytes the block should have
/// @param list The list of memory blocks on this memory page
/// @return The block of memory with enough available space or
///         NULL if none of the mentioned steps were successful
t_small_block* reallocate_small_block(
    t_small_block* block,
    const size_t size,
    t_small_block* list
);

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

/// @brief Returns the next block in memory on the current memory page
/// @param block A pointer to the current block of memory
/// @return The next block in memory or
///         NULL if the current block is the last block on the
///         current memory page
t_small_block* get_next_small_block(t_small_block* block);

/// @brief Returns the previous block in memory on the current memory page
/// @param block A pointer to the current block of memory
/// @return The previous block in memory or
///         NULL if the current block is the first block on the
///         current memory page
t_small_block* get_prev_small_block(t_small_block* block);

/// @brief Determines whether overflow has occurred
///        in a block of memory
/// @param block The memory block that should be checked
/// @return True if overflow has occured and
///         false if not
bool small_block_is_corrupted(t_small_block* block);

// ---------------------- reorganize_small_block.c -----------------

/// @brief Coalesces the block of memory with its neighbours if they
///        are not in use
/// @param block The memory block to merge.
///              If `merge_backwards` is set to true the address
///              of the block might change
/// @param merge_backwards A flag that decides whether `block` should
///                        be merged with its previous neighbours
/// @return True if at least one merge was performed and
///         false if no merge was possible
bool merge_small_block(t_small_block** block, bool merge_backwards);

/// @brief Splits one block of memory in two at the specified
///        split point if there is enough space availabe in the block.
///        The newly created block is marked as availabe
/// @param block The block that might be splitted
/// @param split_size The size the block should have after
///                   spliting
/// @return True if the block could be splitted or
///         false if the size of the block was too small
bool split_small_block(t_small_block* block, const size_t split_size);
