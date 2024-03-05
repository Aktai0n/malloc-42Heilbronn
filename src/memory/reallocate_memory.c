
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
    if (next == NULL || is_allocated(next)) {
        return false;
    }
    if (new_size > get_alloc_size(block) + get_alloc_size(next) + sizeof(*next)) {
        return false;
    }
    if (!merge_alloc_block(block, free_list)) {
        return false;
    }
    if (is_last_block(block)) {
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
    //     if (is_last_block(block)) {
    //         split_alloc_block(block, size, &page->free_list);
    //     }
    //     return ptr;
    // }

    void* new_block = reallocate_and_copy_(block, size);
    release_memory(ptr);
    return new_block;
}
