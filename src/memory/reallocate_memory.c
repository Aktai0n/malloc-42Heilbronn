
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
    return (void*)(new_block + 1);
}

void* reallocate_memory(void* ptr, size_t size) {
    size = ALIGN_ALLOC_SIZE(size);
    t_alloc_block* block = get_alloc_block(ptr);
    t_memory_page* page = find_memory_page(block);
    if (page == NULL) {
        errno = EINVAL;
        return NULL;
    }

    size_t block_size = get_alloc_size(block);
    if (block_size >= size) {
        return ptr;
    }

    // try to expand the size of the current block
    // TODO! refactor merge and split alloc block so they
    // can be used here!
    if (merge_alloc_block(block, &page->free_list)) {
        if (is_last_block(block)) {
            split_alloc_block(block, size, &page->free_list, &page->allocated_list);
        }
        return ptr;
    }

    void* new_block = reallocate_and_copy_(block, size);
    release_memory(ptr);
    return new_block;
}
