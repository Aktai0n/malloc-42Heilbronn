
#include <stdbool.h>

#include "memory_page.h"
#include "../alloc_block/alloc_block.h"

/// @brief Constructs a single free memory block whose size is 
///        (almost) the entire page
/// @param page the memory page the block should be added to
static void init_free_list_(t_memory_page* page) {
    t_alloc_block* free_block = (t_alloc_block*)(page + 1);
    free_block->next = free_block;
    free_block->prev = free_block;
    free_block->size = page->size - (sizeof(*page) + sizeof(*free_block));
    set_alloc_block_flag(free_block, IS_ALLOCATED_FLAG, false);
    set_alloc_block_flag(free_block, IS_LAST_BLOCK_FLAG, true);
    page->free_list = free_block;
}

/// @brief Constructs a new memory page to be used for allocations
/// @param size The size of the new page
/// @param type The type of the new page (determines the size
///             of the stored blocks)
/// @param additional_mmap_flags Additional flags passed to mmap().
///                              MAP_PRIVATE and MAP_ANONYMOUS are
///                              always enabled
/// @return the newly created memory page or NULL if mmap fails
t_memory_page* init_memory_page(
    size_t size,
    enum e_memory_page type,
    int additional_mmap_flags
) {
    t_memory_page* page = call_mmap(
        size,
        additional_mmap_flags
    );
    if (page == NULL) {
        return NULL;
    }
    page->type = type;
    page->size = size - sizeof(*page);
    page->allocated_list = NULL;
    page->next = NULL;
    init_free_list_(page);
    return page;
}

