
#include "ft_malloc.h"
#include "defines.h"
#include "memory_page/memory_page.h"


enum e_memory_page find_page_type(size_t size) {
    if (size <= TINY_ALLOC_BLOCK_SIZE) {
        return TINY_PAGE;
    } else if (size <= SMALL_ALLOC_BLOCK_SIZE) {
        return SMALL_PAGE;
    } else {
        return LARGE_PAGE;
    }
}

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

static t_alloc_block* search_alloc_block_(t_memory_page* page, const size_t size) {
    if (page == NULL) {
        return NULL;
    }
    t_alloc_block* block = init_alloc_block(
        size,
        &page->free_list,
        &page->allocated_list
    );
    return block;
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

void* allocate_memory(size_t requested_block_size) {
    requested_block_size = ALIGN_ALLOC_SIZE(requested_block_size);

    t_alloc_block* block = find_existing_alloc_block_(requested_block_size);
    if (block != NULL) {
        return (void*)((size_t)block + sizeof(*block));
    }

    t_memory_page* page = NULL;
    if (requested_block_size <= TINY_ALLOC_BLOCK_SIZE) {
        page = create_memory_page_(TINY_PAGE_SIZE, TINY_PAGE, 0, &g_heap.tiny_pages);
    } else if (requested_block_size <= SMALL_ALLOC_BLOCK_SIZE) {
        page = create_memory_page_(SMALL_PAGE_SIZE, SMALL_PAGE, 0, &g_heap.small_pages);
    } else {
        page = create_memory_page_(requested_block_size, LARGE_PAGE, 0, &g_heap.large_pages);
    }
    if (page == NULL) {
        return NULL;
    }
    block = init_alloc_block(requested_block_size, &page->free_list, &page->allocated_list);
    if (block == NULL) {
        return NULL;
    }
    return (void*)((size_t)block + sizeof(*block));
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

void* allocate_memory_bzero(size_t requested_block_size, bool set_zero) {
    void* ptr = allocate_memory(requested_block_size);
    if (!set_zero) {
        return ptr;
    }
    t_alloc_block* block = (t_alloc_block*)((size_t)ptr - sizeof(t_alloc_block));
    bzero_calloc_internal_(block);
    return ptr;
}
