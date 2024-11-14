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
    // if (!PTR_IS_ALIGNED(ptr, FT_MALLOC_ALIGNMENT) &&
    //     !PTR_IS_ALIGNED(ptr, (size_t)getpagesize())) {
    //     errno = EFAULT;
    //     return false;
    // }
    t_small_page* small_page = find_in_small_page_list(ptr, g_heap.small_pages);
    if (small_page != NULL) {
        return deallocate_small(
            get_small_block(ptr),
            small_page,
            &g_heap.small_pages
        );
    }
    t_medium_page* medium_page = find_in_medium_page_list(ptr, g_heap.medium_pages);
    if (medium_page != NULL) {
        return deallocate_medium(
            get_medium_block(ptr),
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
