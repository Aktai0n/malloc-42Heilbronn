

#if 1

#include <errno.h>

#include "memory.h"
#include "defines.h"
#include "ft_malloc_internal.h"
#include "small/small.h"
#include "small/page/small_page.h"
#include "small/block/small_block.h"
#include "medium/medium.h"
#include "medium/page/medium_page.h"
#include "medium/block/medium_block.h"
#include "large/large.h"
#include "large/page/large_page.h"

bool release_memory(void* ptr) {
    if (!PTR_IS_ALIGNED(ptr, FT_MALLOC_ALIGNMENT) &&
        !PTR_IS_ALIGNED(ptr, (size_t)getpagesize())) {
        errno = EFAULT;
        return false;
    }
    t_small_page* small_page = find_in_small_page_list(ptr, g_heap.tiny_pages);
    if (small_page != NULL) {
        return deallocate_small(
            get_small_block(ptr),
            small_page,
            &g_heap.tiny_pages
        );
    }
    t_medium_page* medium_page = find_in_medium_page_list(ptr, g_heap.small_pages);
    if (medium_page != NULL) {
        return deallocate_medium(
            get_medium_block(ptr),
            medium_page,
            &g_heap.small_pages
        );
    }
    t_large_page* large_page = find_in_large_page_list(ptr, &g_heap.large_pages);
    if (large_page != NULL) {
        return deallocate_large(large_page, &g_heap.large_pages);
    }
    errno = EINVAL;
    return false;
}

#else

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

#endif
