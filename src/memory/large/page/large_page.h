#pragma once

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#include "large_page_struct.h"

/// @brief Returns the usable number of bytes in a large memory page
/// @param page The memory page whose size should be determined
/// @return The usable size of the memory page
inline size_t get_large_page_data_size(const t_large_page* page) {
    return page->size - (sizeof(*page) + sizeof(t_large_page_end));
}

/// @brief Returns the data pointer assoiciated to a large page
///        of memory
/// @param page The memory page whose data pointer should
///             be retrieved
/// @return A pointer to the usable data region
///         in the memory page
inline void* get_large_page_data(t_large_page* page) {
    return (void*)((uintptr_t)page + sizeof(*page));
}

/// @brief Returns the large page end structure located at the
///        last bytes of a large page of memory
/// @param page The memory page whose end structure should be
///             retrieved
/// @return The large page end structure associated to the
///         memory page
inline t_large_page_end* get_large_page_end(t_large_page* page) {
    return (t_large_page_end*)(
        (uintptr_t)page + (page->size - sizeof(t_large_page_end))
    );
}

/// @brief Determines whether under- or overflow has occurred
///        in a page of memory
/// @param page The memory page that should be checked
/// @return True if under- or overflow has occured and
///         false if not
inline bool large_page_is_corrupted(t_large_page* page) {
    return get_large_page_end(page)->size != page->size;
}

// ---------------------- large_page_list.c -----------------------

/// @brief Adds a page of memory to the given list of pages
/// @param list The list of pages to be expanded
/// @param new_page The page to be added to the list
void add_to_large_page_list(t_large_page** list, t_large_page* new_page);

/// @brief Removes a page of memory form the given list of pages
/// @param list The list of memory pages to be reduced
/// @param to_remove The memory page that should be removed
///                  from the list
/// @return The removed memory page
///         or NULL if the page isn't found in the page list
t_large_page* delete_from_large_page_list(
    t_large_page** list,
    t_large_page* to_remove
);

/// @brief Looks for the memory region that ptr belongs to
/// @param ptr The pointer to be associated to a page of memory
/// @param list A list of memory pages in which to look for ptr
/// @return The memory page containing ptr or
///         NULL if ptr wasn't found
t_large_page* find_in_large_page_list(
    const void* ptr,
    t_large_page* list
);

// ---------------------- large_page_operations.c -----------------

/// @brief Constructs a new large page of memory to be used for allocations
/// @param size The usable size of the new page
/// @param additional_mmap_flags Additional flags passed to mmap.
///                              MAP_PRIVATE and MAP_ANONYMOUS are
///                              always enabled
/// @param page_list The list of pages the newly created page
///                  should be added to
/// @return the newly created memory page or NULL if mmap fails
t_large_page* create_large_page(
    size_t size,
    int additional_mmap_flags,
    t_large_page** page_list
);

/// @brief Deletes the page of memory from its page list
///        and releases the memory
/// @param page The page to be deleted
/// @param page_list The list of memory pages to shrink
/// @return True if the page is sucessfuly deleted or
///         false if the page is not found or
///         wasn't successfully released
bool destroy_large_page(
    t_large_page* page,
    t_large_page** page_list
);

/// @brief Changes the size of the given page of memory to fit at least
///        size bytes
/// @param page The page to be modified
/// @param size The new size of the memory page
/// @param page_list The list of memory pages page is inside
/// @return The memory page that has at least size bytes of capacity
///         or NULL if the creation of a new page was unsuccessful
t_large_page* realloc_large_page(
    t_large_page* page,
    const size_t size,
    t_large_page** page_list
);
