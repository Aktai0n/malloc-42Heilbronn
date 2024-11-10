#include "ft_malloc_internal.h"
#include "defines.h"
#include "small/small.h"
#include "small/block/small_block.h"
#include "medium/medium.h"
#include "medium/block/medium_block.h"
#include "large/large.h"
#include "large/page/large_page.h"

void* allocate_memory(size_t size, bool set_zero) {
    size = ALIGN_ALLOC_SIZE(size, FT_MALLOC_ALIGNMENT);
    // if (size <= TINY_ALLOC_BLOCK_SIZE) {
    //     t_small_block* block = allocate_small(
    //         size,
    //         set_zero,
    //         &g_heap.small_pages
    //     );
    //     if (block == NULL) {
    //         return NULL;
    //     }
    //     return get_small_block_data(block);
    if (size <= SMALL_ALLOC_BLOCK_SIZE) {
        t_medium_block* block = allocate_medium(
            size,
            set_zero,
            &g_heap.medium_pages
        );
        if (block == NULL) {
            return NULL;
        }
        return get_medium_block_data(block);
    } else {
        t_large_page* page = allocate_large(size, &g_heap.large_pages);
        if (page == NULL) {
            return NULL;
        }
        return get_large_page_data(page);
    }
}
