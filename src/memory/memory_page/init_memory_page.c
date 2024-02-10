
#include <stdbool.h>

#include "memory_page.h"
#include "../alloc_block/alloc_block.h"

/// @brief Constructs a single free memory block whose size is 
///        (almost) the entire page
/// @param page the memory page the block should be added to
static void init_free_list_(t_memory_page* page) {
    t_alloc_block* free_block = (t_alloc_block*)((size_t)page + sizeof(*page));
    free_block->next = NULL;
    free_block->size = page->size - (sizeof(*free_block) + sizeof(*page));
    set_alloc_block_flag(free_block, IS_ALLOCATED_FLAG, false);
    set_alloc_block_flag(free_block, IS_LAST_BLOCK_FLAG, true);
    page->free_list = free_block;
}

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
    page->size = size;
    page->allocated_list = NULL;
    page->next = NULL;
    init_free_list_(page);
    return page;
}
