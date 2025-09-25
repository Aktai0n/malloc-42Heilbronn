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
    t_small_page* small_page = find_in_small_page_list(ptr, g_heap.small_pages);
    if (small_page != NULL) {
        t_small_block* block = find_in_small_block_list(ptr, small_page->block_list);
        if (block == NULL) {
            errno = EINVAL;
            return false;
        }
        return deallocate_small(
            block,
            small_page,
            &g_heap.small_pages
        );
    }
    t_medium_page* medium_page = find_in_medium_page_list(ptr, g_heap.medium_pages);
    if (medium_page != NULL) {
        t_medium_block* block = find_in_medium_block_list(ptr, medium_page->allocated_list);
        if (block == NULL) {
            errno = EINVAL;
            return false;
        }
        return deallocate_medium(
            block,
            medium_page,
            &g_heap.medium_pages
        );
    }
    t_large_page* large_page = find_in_large_page_list(ptr, g_heap.large_pages);
    if (large_page != NULL) {
        return deallocate_large(large_page, &g_heap.large_pages);
    }
    errno = EINVAL;
    return false;
}
