#include <errno.h>

#include "ft_malloc_internal.h"
#include "defines.h"
#include "small/small.h"
#include "small/page/small_page.h"
#include "small/block/small_block.h"
#include "medium/medium.h"
#include "medium/block/medium_block.h"
#include "medium/page/medium_page.h"
#include "large/large.h"
#include "large/page/large_page.h"

static void* realloc_small_(
    t_small_block* old_block,
    size_t size,
    t_small_page* page,
    struct s_heap* heap
) {
    void* new_data = NULL;
    void* old_data = get_small_block_data(old_block);
    size_t old_size = get_block_size(old_block->curr);
    if (size <= TINY_ALLOC_BLOCK_SIZE) {
        t_small_block* new_block = reallocate_small(
            old_block,
            size,
            page,
            &heap->small_pages
        );
        if (new_block != NULL) {
            return get_small_block_data(new_block);
        }
    } else if (size <= SMALL_ALLOC_BLOCK_SIZE) {
        t_medium_block* new_block = allocate_medium(
            size,
            false,
            &heap->medium_pages
        );
        if (new_block != NULL) {
            new_data = get_medium_block_data(new_block);
        }
    } else {
        t_large_page* new_page = allocate_large(size, &g_heap.large_pages);
        if (new_page != NULL) {
            new_data = get_large_page_data(new_page);
        }
    }

    if (new_data != NULL) {
        size = size < old_size ? size : old_size;
        ft_malloc_memcpy(old_data, new_data, size);
        deallocate_small(old_block, page, &heap->small_pages);
    }
    return new_data;
}

static void* realloc_medium_(
    t_medium_block* old_block,
    size_t size,
    t_medium_page* page,
    struct s_heap* heap
) {
    void* new_data = NULL;
    void* old_data = get_medium_block_data(old_block);
    size_t old_size = get_block_size(old_block->curr);
    if (size <= SMALL_ALLOC_BLOCK_SIZE) {
        t_medium_block* new_block = reallocate_medium(
            old_block,
            size,
            page,
            &heap->medium_pages
        );
        if (new_block != NULL) {
            return get_medium_block_data(new_block);
        }
    } else {
        t_large_page* new_page = allocate_large(size, &g_heap.large_pages);
        if (new_page != NULL) {
            new_data = get_large_page_data(new_page);
        }
    }

    if (new_data != NULL) {
        size = size < old_size ? size : old_size;
        ft_malloc_memcpy(old_data, new_data, size);
        deallocate_medium(old_block, page, &heap->medium_pages);
    }
    return new_data;
}

static void* realloc_large_(
    size_t size,
    t_large_page* page,
    struct s_heap* heap
) {
    void* new_data = NULL;
    size_t old_size = get_large_page_data_size(page);
    void* old_data = get_large_page_data(page);
    if (size <= TINY_ALLOC_BLOCK_SIZE) {
        t_small_block* new_block = allocate_small(
            size,
            false,
            &heap->small_pages
        );
        if (new_block != NULL) {
            new_data = get_small_block_data(new_block);
        }
    } else if (size <= SMALL_ALLOC_BLOCK_SIZE) {
        t_medium_block* new_block = allocate_medium(
            size,
            false,
            &heap->medium_pages
        );
        if (new_block != NULL) {
            new_data = get_medium_block_data(new_block);
        }
    } else {
        t_large_page* new_page = reallocate_large(
            page,
            size,
            &heap->large_pages
        );
        if (new_page != NULL) {
            return get_large_page_data(new_page);
        }
    }

    if (new_data != NULL) {
        size = size < old_size ? size : old_size;
        ft_malloc_memcpy(new_data, old_data, size);
        deallocate_large(page, &heap->large_pages);
    }
    return new_data;
}

void* reallocate_memory(void* ptr, size_t size) {
    size = ALIGN_ALLOC_SIZE(size, FT_MALLOC_ALIGNMENT);
    struct s_heap* heap = &g_heap;

    t_small_page* small_page = find_in_small_page_list(ptr, heap->small_pages);
    if (small_page != NULL) {
        t_small_block* block = find_in_small_block_list(ptr, small_page->block_list);
        if (block == NULL) {
            errno = EINVAL;
            return NULL;
        }
        return realloc_small_(block, size, small_page, heap);
    }
    t_medium_page* medium_page = find_in_medium_page_list(ptr, heap->medium_pages);
    if (medium_page != NULL) {
        t_medium_block* block = find_in_medium_block_list(ptr, medium_page->allocated_list);
        if (block == NULL) {
            errno = EINVAL;
            return NULL;
        }
        return realloc_medium_(block, size, medium_page, heap);
    }
    t_large_page* large_page = find_in_large_page_list(ptr, heap->large_pages);
    if (large_page != NULL) {
        return realloc_large_(size, large_page, heap);
    }
    errno = EINVAL;
    return NULL;
}
