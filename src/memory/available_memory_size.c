
#include <errno.h>

#if 1

#include "ft_malloc_internal.h"
#include "small/page/small_page.h"
#include "small/block/small_block.h"
#include "medium/page/medium_page.h"
#include "medium/block/medium_block.h"
#include "large/page/large_page.h"

size_t available_memory_size(void* ptr, struct s_heap* heap) {
    t_small_page* small_page = find_in_small_page_list(ptr, heap->tiny_pages);
    if (small_page != NULL) {
        t_small_block* small_block = get_small_block(ptr);
        return get_block_size(small_block->curr);
    }
    t_medium_page* medium_page = find_in_medium_page_list(ptr, heap->small_pages);
    if (medium_page != NULL) {
        t_medium_block* medium_block = get_medium_block(ptr);
        return get_block_size(medium_block->curr);
    }
    t_large_page* large_page = find_in_large_page_list(ptr, heap->large_pages);
    if (large_page != NULL) {
        return get_large_page_data_size(large_page);
    }
    errno = EINVAL;
    return 0;
}



#else
#include "memory_page/memory_page.h"
#include "alloc_block/alloc_block.h"


size_t available_memory_size(void* ptr) {
    t_alloc_block* block = (t_alloc_block*)ptr - 1;
    t_memory_page* page = find_memory_page(block);
    if (page == NULL) {
        errno = EINVAL;
        return 0;
    }
    return block->size;
}

#endif