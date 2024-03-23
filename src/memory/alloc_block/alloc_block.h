#pragma once

#include <stddef.h>
#include <stdbool.h>

#include "alloc_block_struct.h"

#define IS_ALLOCATED_FLAG ((size_t)0b1)
#define IS_LAST_BLOCK_FLAG ((size_t)0b10)
#define ALLOC_FLAGS (IS_ALLOCATED_FLAG | IS_LAST_BLOCK_FLAG)

// ---------------------- inline functions -----------------------

/// @brief Returns the true size of a block of memory by
///        masking away the allocation flags
/// @param block The block whose size is returned
/// @return The usable size of the block
inline size_t get_alloc_size(const t_alloc_block* block) {
    return block->size & ~ALLOC_FLAGS;
}

/// @brief sets the size of a block of memory without affecting
///        the allocation flags
/// @param block The block whose size should be changed
/// @param size The new usable size of the block
inline void set_alloc_size(t_alloc_block* block, const size_t size) {
    block->size = (block->size & ALLOC_FLAGS) | (size & ~ALLOC_FLAGS);
}

/// @brief determines whether the block is currently in use or not
inline bool is_allocated_block(const t_alloc_block* block) {
    return block->size & IS_ALLOCATED_FLAG;
}

/// @brief determines whether this block is the last in memory
///        on the current memory page
inline bool is_last_alloc_block(const t_alloc_block* block) {
    return block->size & IS_LAST_BLOCK_FLAG;
}

/// @brief Sets or resets a flag in the specified memory block
/// @param block The memory block to set the flag in
/// @param flag The flag that should be modified
/// @param state The state to which the flag will be set to
inline void set_alloc_block_flag(
    t_alloc_block* block,
    size_t flag,
    bool state
) {
    if (state == true) {
        block->size = block->size | flag;
    } else {
        block->size = block->size & ~flag;
    }
}

/// @brief Returns the alloc_block structure associated to
///        an allocated block of memory
/// @param ptr A pointer pointing to the usable memory in
///            the alloc_block structure
/// @return The structure associated to the memory
inline t_alloc_block* get_alloc_block(void* ptr) {
    return (t_alloc_block*)((size_t)ptr - sizeof(t_alloc_block));
}

/// @brief Returns the data pointer assoiciated to a block
///        of memory
/// @param block The memory block whose data pointer should
///              be retrieved
/// @return A pointer to the usable data region
///         in the memory block
inline void* get_alloc_data(t_alloc_block* block) {
    return (void*)((size_t)block + sizeof(*block));
}

/// @brief Returns the next block in memory on the current memory page
/// @param block A pointer to the current block of memory
/// @return The next block in memory or
///         NULL if the current block is the last block on the
///         current memory page
inline t_alloc_block* get_next_block_in_memory(t_alloc_block* block) {
    if (is_last_alloc_block(block)) {
        return NULL;
    }
    return (t_alloc_block*)((size_t)block + sizeof(*block) + get_alloc_size(block));
}

// ---------------------- alloc_block_list.c -----------------------

/// @brief Adds a memory block to the given list of blocks
/// @param list The list of blocks to be expanded
/// @param new_block The block to be added to the list
void add_to_alloc_list(t_alloc_block** list, t_alloc_block* new_block);

/// @brief Removes a memory block form the given list of blocks
/// @param list The list of memory blocks to be reduced
/// @param to_remove The memory block that should be removed
///                  from the list
/// @return The removed memory block
///         or NULL if the block isn't found in the block list
t_alloc_block* delete_from_alloc_list(
    t_alloc_block** list,
    t_alloc_block* to_remove
);

/// @brief Finds a free block of memory to allocate
///        using a best fit approach
/// @param list_head The list of blocks to search in
/// @param size The smallest size the block should have
/// @return A memory block that has at least size bytes or
///         NULL if no block is found in the list
t_alloc_block* find_alloc_block(
    t_alloc_block* list_head,
    const size_t size
);

// ---------------------- allocate_alloc_block.c ---------------------

/// @brief Searches for an available block of memory
///        with at least size bytes of capacity
///        in the given free list and adds it
///        to the given allocaed list
/// @param size The minimal available size
///             of the memory block
/// @param free_list A list of free memory blocks
///                  to search in
/// @param allocated_list The list of memory blocks
///                       the newly allocated block
///                       is added to
/// @return The reserved block of memory or
///         NULL if no available block was found
///         in the free list
t_alloc_block* allocate_alloc_block(
    const size_t size,
    t_alloc_block** free_list,
    t_alloc_block** allocated_list
);

// ---------------------- reclaim_alloc_block.c ---------------------

/// @brief Reclaims the block of memory as available by removing it
///        from the list of allocated blocks and adding it to the
///        list of free blocks. If possible, the memory is defragmented
/// @param block The block to be reclaimed
/// @param free_list A list of free memory blocks
/// @param allocated_list The list of allocated memory blocks the
///                       block to be reclaimed is part of
/// @return True if the block was successfully reclaimed or
///         false if the block was not part of the allocated list
bool reclaim_alloc_block(
    t_alloc_block* block,
    t_alloc_block** free_list,
    t_alloc_block** allocated_list
);

// ---------------------- merge_alloc_block.c ---------------------

/// @brief Defragments memory by merging the given block
///        with its next block in memory
///        if the next block is not in use
/// @param block The block that is checked for merging
/// @param free_list The free list in which the next block
///                  is stored before the merge
/// @return True if the merge was performed successfully or
///         false if there is no next block or
///         the next block is in use
bool merge_alloc_block(
    t_alloc_block* block,
    t_alloc_block** free_list
);

// ---------------------- split_alloc_block.c ---------------------

/// @brief Splits one memory block into two at the specified
///        split size if there is enough available size
///        in the block
/// @param block The block to be splitted
/// @param split_size The size the block should have after
///                   spliting
/// @param free_list The list in which the newly created
///                  block is stored
/// @return True if the block could be splitted or
///         false if the size of the block was too small
t_alloc_block* split_alloc_block(
    t_alloc_block* block,
    const size_t split_size,
    t_alloc_block** free_list
);
