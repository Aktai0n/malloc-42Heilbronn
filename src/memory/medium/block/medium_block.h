#pragma once

#include <stddef.h>
#include <stdint.h>

#include "medium_block_struct.h"
#include "utils.h"


// ---------------------- inline functions -----------------------

/// @brief returns the medium block structure associated to
///        an allocated block of memory
/// @param ptr A pointer pointing to the usable memory in
///            the medium block structure
/// @return The structure associated to the memory
inline t_medium_block* get_medium_block(void* ptr) {
    return (t_medium_block*)((uintptr_t)ptr - sizeof(t_medium_block));
}

/// @brief Returns the data pointer assoiciated to a
///        medium block of memory
/// @param block The memory block whose data pointer should
///              be retrieved
/// @return A pointer to the usable data region
///         in the memory block
inline void* get_medium_block_data(t_medium_block* block) {
    return (void*)((uintptr_t)block + sizeof(*block));
}

/// @brief Returns a pointer pointing to the address just behind
///        the last data byte in the given block. 
///        Essentially returning an `end iterator`
/// @param block The block whose end should be calculated
/// @return A pointer pointing behind the data region in the
///         memory block
/// @attention Dereferencing this pointer is undefined behaviour
inline void* get_medium_block_data_end(t_medium_block* block) {
    return (void*)(
        (uintptr_t)get_medium_block_data(block) +
        get_block_size(block->curr)
    );
}

inline void copy_medium_block_data(
    t_medium_block* src,
    t_medium_block* dst
) {
    ft_malloc_memcpy(
        get_medium_block_data(src),
        get_medium_block_data(dst),
        get_block_size(src->curr)
    );
}

// ---------------------- medium_block_operations.c ---------------

/// @brief Searches for an available block of memory with at least
///        `size` bytes of capacity in the given free list,
///        marks it as in use and appends it to the allocated list
/// @param size The minimal available bytes the block should have
/// @param free_list The list of free memory blocks on this memory page
///                  the new block will be taken from
/// @param allocated_list The list of allocated memory blocks on this
///                       memory page the new block will be added to
/// @return The reserved block of memory or
///         NULL if no fitting block was found
t_medium_block* allocate_medium_block(
    const size_t size,
    t_medium_block** free_list,
    t_medium_block** allocated_list
);

/// @brief Resets the block of memory as available, removes it from
///        the alloced list, adds it to the free list and
///        defragments the memory if possible.
///        If defragmentation is performed, the address of `block`
///        might change
/// @param block The memory block to be reclaimed
/// @param free_list The list of free blocks that `block` is added to
/// @param allocated_list The list of allocated blocks that `block`
///                       will be removed from
/// @return True if the block was successfully reclaimed or
///         False if he wasn't found in the `allocated_list`
bool deallocate_medium_block(
    t_medium_block** block,
    t_medium_block** free_list,
    t_medium_block** allocated_list
);

/// @brief Attempts to increase the size of the given
///        block of memory by:
///        1. Checking whether it has enough space available.
///        2. Trying to merge it with its next block in memory
///        3. Reserving a new block on the current memory page
///           and copying the contents over
/// @param block The memory block whose size should be increased
/// @param size The minimal number of bytes the block should have
/// @param free_list The list of free blocks on this memory page
/// @param allocated_list The list of allocated blocks on this memory page
/// @return The block of memory with enough available space or
///         NULL if none of the mentioned steps were successful
t_medium_block* reallocate_medium_block(
    t_medium_block* block,
    const size_t size,
    t_medium_block** free_list,
    t_medium_block** allocated_list
);

// ---------------------- medium_block_list.c ---------------------

/// @brief Returns the next block in memory on the current memory page
/// @param block A pointer to the current block of memory
/// @return The next block in memory or
///         NULL if the current block is the last block on the
///         current memory page
t_medium_block* get_next_medium_block(t_medium_block* block);

/// @brief Returns the previous block in memory on the current memory page
/// @param block A pointer to the current block of memory
/// @return The previous block in memory or
///         NULL if the current block is the first block on the
///         current memory page
t_medium_block* get_prev_medium_block(t_medium_block* block);

/// @brief Looks for the memory block that ptr belongs to
/// @param ptr The pointer to be associated to a memory block
/// @param list A list of memory blocks in which to look for ptr
/// @return The memory block associated to ptr or
///         NULL if ptr wasn't found
t_medium_block* find_in_medium_block_list(const void* ptr, t_medium_block* list);

/// @brief Finds a free block of memory to allocate
///        using a best fit approach
/// @param list The list of available blocks to search in
/// @param size The smallest size the block should have
/// @return A memory block that has at least size bytes or
///         NULL if no block is found in the list
t_medium_block* find_free_medium_block(
    t_medium_block* list,
    const size_t size
);

/// @brief Adds a block of memory to the given list of blocks
/// @param list The list of blockss to be expanded
/// @param to_add The block to be added to the list
void add_to_medium_block_list(
    t_medium_block** list,
    t_medium_block* to_add
);

/// @brief Removes a block of memory form the given list of blocks
/// @param list The list of memory blocks to be reduced
/// @param to_remove The memory block that should be removed
///                  from the list
/// @return The removed memory block
///         or NULL if the block isn't found in the block list
t_medium_block* delete_from_medium_block_list(
    t_medium_block** list,
    t_medium_block* to_remove
);

/// @brief Determines whether overflow has occurred
///        in a block of memory
/// @param block The memory block that should be checked
/// @return True if overflow has occured and
///         false if not
bool medium_block_is_corrupted(t_medium_block* block);

// ---------------------- reorganize_medium_block.c ---------------

/// @brief Coalesces the block of memory with its neighbours if they
///        are not in use
/// @param block The memory block to merge.
///              If `merge_backwards` is set to true the address
///              of the block might change
/// @param merge_backwards A flag that decides whether `block` should
///                        be merged with its previous neighbours
/// @param free_list The free blocks on the current memory page.
///                  The neighbours of `block` will be removed from this
///                  list before the merging happens
/// @return True if at least one merge was performed and
///         false if no merge was possible
bool merge_medium_block(
    t_medium_block** block,
    bool merge_backwards,
    t_medium_block** free_list
);

/// @brief Splits one block of memory in two at the specified
///        split point if there is enough space availabe in the block.
///        The newly created block is marked as availabe and
///        added to the free list
/// @param block The block that might be splitted
/// @param split_size The size the block should have after
///                   spliting
/// @param free_list The list of free blocks on this memory page.
///                  The new block will be added to this list after
///                  its creation
/// @return True if the block could be splitted or
///         false if the size of the block was too small
bool split_medium_block(
    t_medium_block* block,
    const size_t split_size,
    t_medium_block** free_list
);
