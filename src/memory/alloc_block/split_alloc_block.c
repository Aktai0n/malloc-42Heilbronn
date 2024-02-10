
#include "defines.h"
#include "alloc_block.h"

static bool can_be_splitted_(const size_t block_size, const size_t split_size) {
    return block_size >= split_size + FT_MALLOC_ALIGNMENT + sizeof(t_alloc_block);
}

t_alloc_block* split_alloc_block(
    t_alloc_block* block,
    const size_t split_size,
    t_alloc_block** free_list
) {
    // check whether the block is big enough to be splitted
    const size_t block_size = get_alloc_size(block);
    if (!can_be_splitted_(block_size, split_size)) {
        return NULL;
    }

    // create a new free block at the future
    // memory boundary (split_size + sizeof(t_alloc_block)) of the current block
    t_alloc_block* new_block = (t_alloc_block*)((size_t)block + sizeof(*block) + split_size);
    size_t new_block_size = block_size - (split_size + sizeof(*block));
    set_alloc_size(new_block, new_block_size);
    set_alloc_block_flag(new_block, IS_ALLOCATED_FLAG, false);

    // transfer the last block flag over if the current block was the last one
    // on this page
    if (is_last_block(block)) {
        set_alloc_block_flag(new_block, IS_LAST_BLOCK_FLAG, true);
        set_alloc_block_flag(block, IS_LAST_BLOCK_FLAG, false);
    } else {
        set_alloc_block_flag(new_block, IS_LAST_BLOCK_FLAG, false);
    }
    set_alloc_size(block, split_size);
    add_to_alloc_list(free_list, new_block);
    return block;
}
