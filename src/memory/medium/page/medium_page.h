#pragma once

#include <stddef.h>

#include "../block/medium_block_struct.h"
#include "medium_page_struct.h"

// ---------------------- medium_page_list.c -----------------------

/// @brief Adds a page of memory to the given list of pages
/// @param list The list of pages to be expanded
/// @param new_page The page to be added to the list
void add_to_medium_page_list(t_medium_page** list, t_medium_page* new_page);

/// @brief Removes a page of memory form the given list of pages
/// @param list The list of memory pages to be reduced
/// @param to_remove The memory page that should be removed
///                  from the list
/// @return The removed memory page
///         or NULL if the page isn't found in the page list
t_medium_page* delete_from_medium_page_list(
    t_medium_page** list,
    t_medium_page* to_remove
);

/// @brief Looks for the memory region that ptr belongs to
/// @param ptr The pointer to be associated to a page of memory
/// @param list A list of memory pages in which to look for ptr
/// @return The memory page containing ptr or
///         NULL if ptr wasn't found
t_medium_page* find_in_medium_page_list(
    const void* ptr,
    t_medium_page* list
);

// ---------------------- medium_page_operations.c -----------------

/// @brief Constructs a new large page of memory to be used
///        for allocations
/// @param size The usable size of the new page
/// @param additional_mmap_flags Additional flags passed to mmap.
///                              MAP_PRIVATE and MAP_ANONYMOUS are
///                              always enabled
/// @param page_list The list of pages the newly created page
///                  should be added to
/// @return the newly created memory page or NULL if mmap fails
t_medium_page* create_medium_page(
    size_t size,
    int additional_mmap_flags,
    t_medium_page** page_list
);

/// @brief Deletes the page of memory from its page list
///        and releases the memory
/// @param page The page to be deleted
/// @param page_list The list of memory pages to shrink
/// @return True if the page is sucessfuly deleted or
///         false if the page is not found or
///         wasn't successfully released
bool destroy_medium_page(
    t_medium_page* page,
    t_medium_page** page_list
);
