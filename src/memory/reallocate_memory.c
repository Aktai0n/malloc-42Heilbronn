
#if 1

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
    void* old_data,
    size_t size,
    t_small_page* page,
    struct s_heap* heap
) {
    void* new_data = NULL;
    t_small_block* old_block = get_small_block(old_data);
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
    void* old_data,
    size_t size,
    t_medium_page* page,
    struct s_heap* heap
) {
    void* new_data = NULL;
    t_medium_block* old_block = get_medium_block(old_data);
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
    void* old_data,
    size_t size,
    t_large_page* page,
    struct s_heap* heap
) {
    void* new_data = NULL;
    size_t old_size = get_large_page_data_size(page);
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
    if (!PTR_IS_ALIGNED(ptr, FT_MALLOC_ALIGNMENT) &&
        !PTR_IS_ALIGNED(ptr, (size_t)getpagesize())) {
        errno = EFAULT;
        return NULL;
    }
    size = ALIGN_ALLOC_SIZE(size, FT_MALLOC_ALIGNMENT);
    t_small_page* small_page = find_in_small_page_list(ptr, g_heap.small_pages);
    if (small_page != NULL) {
        return realloc_small_(ptr, size, small_page, &g_heap);
    }
    t_medium_page* medium_page = find_in_medium_page_list(ptr, g_heap.medium_pages);
    if (medium_page != NULL) {
        return realloc_medium_(ptr, size, medium_page, &g_heap);
    }
    t_large_page* large_page = find_in_large_page_list(ptr, g_heap.large_pages);
    if (large_page != NULL) {
        return realloc_large_(ptr, size, large_page, &g_heap);
    }
    errno = EINVAL;
    return NULL;
}

#else
#include <errno.h>

#include "ft_malloc.h"
#include "defines.h"
#include "memory.h"
#include "memory_page/memory_page.h"
#include "alloc_block/alloc_block.h"

static void memcpy_realloc_internal_(
    t_alloc_block* new_block,
    t_alloc_block* old_block
) {
    size_t old_size = get_alloc_size(old_block);
    size_t new_size = get_alloc_size(new_block);
    size_t min_size = old_size > new_size ? new_size : old_size;
    size_t* new_ptr = (size_t*)get_alloc_data(new_block);
    size_t* old_ptr = (size_t*)get_alloc_data(old_block);
    for (size_t i = 0; i < min_size; i += sizeof(size_t)) {
        new_ptr[i] = old_ptr[i];
    }
}

static void* reallocate_and_copy_(t_alloc_block* block, size_t new_size) {
    t_alloc_block* new_block = allocate_memory(new_size, false);
    if (new_block == NULL) {
        return NULL;
    }
    memcpy_realloc_internal_(new_block, block);
    return get_alloc_data(new_block);
}

static bool increase_block_size_(
    t_alloc_block* block,
    const size_t new_size,
    t_alloc_block** free_list
) {
    t_alloc_block* next = get_next_block_in_memory(block);
    if (next == NULL || is_allocated_block(next)) {
        return false;
    }
    if (new_size > get_alloc_size(block) + get_alloc_size(next) + sizeof(*next)) {
        return false;
    }
    if (!merge_alloc_block(block, free_list)) {
        return false;
    }
    if (is_last_alloc_block(block)) {
        split_alloc_block(block, new_size, free_list);
    }
    return true;
}

void* reallocate_memory(void* ptr, size_t size) {
    size = ALIGN_ALLOC_SIZE(size, FT_MALLOC_ALIGNMENT);
    t_alloc_block* block = get_alloc_block(ptr);
    t_memory_page* page = find_memory_page(block);
    if (page == NULL) {
        errno = EINVAL;
        return NULL;
    }

    if (get_alloc_size(block) >= size) {
        return ptr;
    }

    // try to expand the size of the current block
    if (increase_block_size_(block, size, &page->free_list)) {
        return ptr;
    }
    // if (merge_alloc_block(block, &page->free_list)) {
    //     if (is_last_alloc_block(block)) {
    //         split_alloc_block(block, size, &page->free_list);
    //     }
    //     return ptr;
    // }

    void* new_block = reallocate_and_copy_(block, size);
    release_memory(ptr);
    return new_block;
}

#endif