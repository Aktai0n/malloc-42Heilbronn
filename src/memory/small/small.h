#pragma once

#include "block/small_block_struct.h"
#include "page/small_page.h"

/// @brief Changes the size of the given block of memory to fit at least
///        size bytes by:
///        1. Calling `reallocate_small_block()`
///        2. Searching in the other memory pages for a suitable block
///           and copying the contents over
///        3. Creating a new memory page and copying the contents over
/// @param block The memory block to be modified
/// @param size The new size of the memory block
/// @param block_list The list of memory blocks that block is inside
/// @param pages The list of small memory pages on this heap
/// @return The memory block with at least size bytes of capacity or
///         NULL if the 
t_small_block* reallocate_small(
    t_small_block* block,
    const size_t size,
    t_small_block* block_list,
    t_small_page** pages
);

/// @brief Attempts to find `ptr` in the given list of small pages and
///        frees it. If it was the last block on the page the whole page
///        is reclaimed
/// @param ptr A pointer to a block of memory to be marked as free
/// @param page_list The list of small memory pages on this heap
/// @return True if the block is found and successfully marked as free.
///         False if the block is not found or an error occurs.
///         In case of error, errno is set to indicate the type of error
bool deallocate_small(void* ptr, t_small_page** page_list);
