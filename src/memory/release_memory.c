
#include <stdbool.h>
#include <errno.h>

#include "memory.h"
#include "alloc_block/alloc_block.h"
#include "memory_page/memory_page.h"

static bool release_alloc_block_(t_memory_page* page, t_alloc_block* block) {
    // delete the block from the allocated list and add it to the free list
    if (delete_from_alloc_list(&page->allocated_list, block) == NULL) {

        return false;
    }
    set_alloc_block_flag(block, IS_ALLOCATED_FLAG, false);
    add_to_alloc_list(&page->free_list, block);

    // try to merge this block with the next in memory (avoid fragmentation)
    merge_alloc_block(block, &page->free_list);
    return true;
}

bool release_memory(void* ptr) {
    t_alloc_block* block = get_alloc_block(ptr);
    t_memory_page* page = find_memory_page(block);
    if (page == NULL) {
        errno = EINVAL;
        return false;
    } else if (!is_allocated_block(block)) {
        // double free
        errno = EACCES;
        return false;
    }

    if (page->next != NULL &&
        (page->allocated_list == NULL || page->allocated_list->next == NULL)) {
        return reclaim_memory_page(page);
    }
    return reclaim_alloc_block(block, &page->free_list, &page->allocated_list);
}
