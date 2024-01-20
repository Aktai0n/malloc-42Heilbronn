#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "../alloc_block/alloc_block_struct.h"
#include "memory_page_struct.h"
#include "memory_page_enum.h"


// ---------------------- abi_wrappers.c -------------------------

/// @brief A wrapper for munmap
/// @param page The memory page to be released
/// @return True if munmap succeds and False if it fails
bool call_munmap(t_memory_page* page);

/// @brief A wrapper for mmap
/// @param size The size to be mapped
/// @param additional_mmap_flags Additional flags passed to mmap.
///                              MAP_PRIVATE and MAP_ANONYMOUS are
///                              always enabled
/// @return The mapped block of memory or
///         NULL if mmap fails
void* call_mmap(size_t size, int additional_flags);

// ---------------------- init_memory_page.c ---------------------

/// @brief Constructs a new memory page to be used for allocations
/// @param size The usable size of the new page
/// @param type The type of the new page (determines the size
///             of the stored blocks)
/// @param additional_mmap_flags Additional flags passed to mmap.
///                              MAP_PRIVATE and MAP_ANONYMOUS are
///                              always enabled
/// @return the newly created memory page or NULL if mmap fails
t_memory_page* init_memory_page(
    size_t size,
    enum e_memory_page type,
    int additional_mmap_flags
);

// ---------------------- destroy_memory_page.c ------------------

/// @brief Deletes the memory page from its page list
///        and releases the memory
/// @param page The page to be deleted
/// @return true if the page is sucessfuly deleted or
///         false if the page is not found or
///         wasn't successfully released
bool destroy_memory_page(t_memory_page* page);

// ---------------------- find_memory_page.c ---------------------

/// @brief Scans the open memory pages for the given block
/// @param block The block whose associated memory page
///              should be located
/// @return The memory page that contains the block
///         or NULL if no page containing the block is found
t_memory_page* find_memory_page(t_alloc_block* block);

// ---------------------- memory_page_list.c ---------------------

/// @brief Removes a memory page form the given list of pages
/// @param list The list of memory pages to be reduced
/// @param to_remove The memory page that should be removed from the list
/// @return The removed memory page
///         or NULL if the page isn't found in the page list
t_memory_page* delete_from_page_list(
    t_memory_page** list,
    t_memory_page* to_remove
);

/// @brief Adds a memory page to the given list of pages
/// @param list The list of memory pages to be expanded
/// @param new_page The memory page to be added to the list
void add_to_page_list(t_memory_page** list, t_memory_page* new_page);
