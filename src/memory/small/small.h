#pragma once

#include <stdbool.h>

#include "block/small_block_struct.h"
#include "page/small_page.h"

/// @brief Searches for an available block of memory with at least
///        `size` bytes of capacity in the given list of memory pages
///        and marks it as in use.
///        If no available block of sufficient size is found a new 
///        memory page is created, added to the list and a block is
///        created in it
/// @param size The minimal available bytes the block should have
/// @param set_zero If set to True the data segmen in the block 
///                 will be zeroed
/// @param pages The list of memory pages in which to search for
///              an available block
/// @return The reserved block of memory or
///         NULL if no new page could be created to accomidate the block
t_small_block* allocate_small(
    const size_t size,
    const bool set_zero,
    t_small_page** pages
);

/// @brief Changes the size of the given block of memory to fit at least
///        size bytes by:
///        1. Calling `reallocate_small_block()`
///        2. Searching in the other memory pages for a suitable block
///           and copying the contents over
///        3. Creating a new memory page and copying the contents over
/// @param block The memory block to be modified
/// @param size The new size of the memory block
/// @param page The memory page `block` is on
/// @param pages The list of small memory pages on this heap
/// @return The memory block with at least size bytes of capacity or
///         NULL if all the mentioned operations fail
t_small_block* reallocate_small(
    t_small_block* block,
    const size_t size,
    t_small_page* page,
    t_small_page** pages
);

/// @brief Frees the given block of memory that's located on the given
///        page of memory. If it was the last block on the page
///        the whole page is reclaimed
/// @param block The memory block to be freed
/// @param page The memory page `block` is located on
/// @param page_list The list of small memory pages on this heap
/// @return True if the block successfully marked as free or
///         False if the block was not in use or
///         an error occured in the freeing process
bool deallocate_small(
    t_small_block* block,
    t_small_page* page,
    t_small_page** page_list
);
