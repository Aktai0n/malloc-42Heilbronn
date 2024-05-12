

#if 1

#include "ft_malloc.h"
#include "defines.h"
#include "small/small.h"
#include "medium/medium.h"
#include "medium/block/medium_block.h"
#include "small/block/small_block.h"

void* allocate_memory(size_t size, bool set_zero) {
    size = ALIGN_ALLOC_SIZE(size, FT_MALLOC_ALIGNMENT);
    if (size <= TINY_ALLOC_BLOCK_SIZE) {
        t_small_block* block = allocate_small(
            size,
            set_zero,
            &g_heap.tiny_pages
        );
        if (block == NULL) {
            return NULL;
        }
        return get_small_block_data(block);
    } else if (size <= SMALL_ALLOC_BLOCK_SIZE) {
        t_medium_block* block = allocate_medium(
            size,
            set_zero,
            &g_heap.small_pages
        );
        if (block == NULL) {
            return NULL;
        }
        return get_medium_block_data(block);
    } else {
        // TODO: allocate large page
    }
}


#else
#include "ft_malloc.h"
#include "defines.h"
#include "memory_page/memory_page.h"
#include "alloc_block/alloc_block.h"

static void bzero_calloc_internal_(t_alloc_block* block);

static t_memory_page* create_memory_page_(
    const size_t size,
    enum e_memory_page type,
    int additional_mmap_flags,
    t_memory_page** page_list
) {
    t_memory_page* page = init_memory_page(size, type, additional_mmap_flags);
    if (page == NULL) {
        return NULL;
    }
    add_to_page_list(page_list, page);
    return page;
}

static t_alloc_block* search_alloc_block_(t_memory_page* pages, const size_t size) {
    for (t_memory_page* page = pages; page != NULL; page = page->next) {
        t_alloc_block* block = allocate_alloc_block(
            size,
            &page->free_list,
            &page->allocated_list
        );
        if (block != NULL) {
            return block;
        }
    }
    return NULL;
}

static void* find_existing_alloc_block_(const size_t size) {
    t_alloc_block* block = NULL;
    if (size <= TINY_ALLOC_BLOCK_SIZE) {
        block = search_alloc_block_(g_heap.tiny_pages, size);
    } else if (size <= SMALL_ALLOC_BLOCK_SIZE) {
        block = search_alloc_block_(g_heap.small_pages, size);
    }
    return block;
}

void* allocate_memory(size_t size, bool set_zero) {
    size = ALIGN_ALLOC_SIZE(size, FT_MALLOC_ALIGNMENT);

    t_alloc_block* block = find_existing_alloc_block_(size);
    if (block != NULL) {
        return get_alloc_data(block);
    }

    t_memory_page* page = NULL;
    if (size <= TINY_ALLOC_BLOCK_SIZE) {
        page = create_memory_page_(TINY_PAGE_SIZE, TINY_PAGE, 0, &g_heap.tiny_pages);
    } else if (size <= SMALL_ALLOC_BLOCK_SIZE) {
        page = create_memory_page_(SMALL_PAGE_SIZE, SMALL_PAGE, 0, &g_heap.small_pages);
    } else {
        size = ALIGN_ALLOC_SIZE(size + sizeof(*page) + sizeof(*block), FT_MALLOC_ALIGNMENT);
        page = create_memory_page_(size, LARGE_PAGE, 0, &g_heap.large_pages);
    }
    if (page == NULL) {
        return NULL;
    }

    block = allocate_alloc_block(size, &page->free_list, &page->allocated_list);
    if (block == NULL) {
        return NULL;
    }

    // large pages are mmaped individually and already zeroed by the kernel
    if (set_zero && page->type != LARGE_PAGE) {
        bzero_calloc_internal_(block);
    }
    return get_alloc_data(block);
}

/// @brief Special implementation for bzero since FT_MALLOC_ALIGNMENT
///        is guranteed to be a multiple of 8
static void bzero_calloc_internal_(t_alloc_block* block) {
    size_t* start = (size_t*)((size_t)block + sizeof(*block));
    size_t* end = (size_t*)((size_t)start + get_alloc_size(block));
    while (start < end) {
        *start = 0;
        ++start;
    }
}

#endif