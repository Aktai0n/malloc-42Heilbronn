#pragma once

#include <stddef.h>
#include <stdbool.h>

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
    return (void*)((size_t)page + sizeof(*page));
}

/// @brief Returns the large page end structure located at the
///        last bytes of a large page of memory
/// @param page The memory page whose end structure should be
///             retrieved
/// @return The large page end structure associated to the
///         memory page
inline t_large_page_end* get_large_page_end(t_large_page* page) {
    return (t_large_page_end*)(
        (size_t)page + (page->size - sizeof(t_large_page_end))
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

void add_to_large_page_list(t_large_page** list, t_large_page* new_page);

t_large_page* delete_from_large_page_list(
    t_large_page** list,
    t_large_page* to_remove
);