
#include "defines.h"
#include "ft_malloc.h"
#include "memory_page.h"
#include "../alloc_block/alloc_block_struct.h"

t_memory_page* scan_memory_pages_(
    t_memory_page* pages,
    t_alloc_block* block
) {
    for (t_memory_page* page = pages; page != NULL; page = page->next) {
        if ((size_t)block > (size_t)page &&
            (size_t)block < (size_t)page + page->size) {
            return page;
        }
    }
    return NULL;
}

t_memory_page* find_memory_page(t_alloc_block* block) {
    if (block->size <= TINY_ALLOC_BLOCK_SIZE) {
        return scan_memory_pages_(g_heap.tiny_pages, block);
    } else if (block->size <= SMALL_ALLOC_BLOCK_SIZE) {
        return scan_memory_pages_(g_heap.small_pages, block);
    } else {
        return scan_memory_pages_(g_heap.large_pages, block);
    }
}
