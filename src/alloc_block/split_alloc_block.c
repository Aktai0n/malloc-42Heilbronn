
#include "defines.h"
#include "alloc_block.h"


t_alloc_block* split_alloc_block(
    t_alloc_block* block,
    size_t split_size,
    t_alloc_block** free_list,
    t_alloc_block** allocated_list
) {
    size_t block_size = get_alloc_size(block);

    // check whether the block is big enough to be splitted
    if (block_size < split_size + FT_MALLOC_ALIGNMENT + sizeof(*block)) {
        return NULL;
    }
    delete_from_alloc_list_(free_list, block);

    // create the new block at the end of the capacity of the current block
    t_alloc_block* new_block = (char*)block + sizeof(*block) + split_size;
    new_block->size = block_size - sizeof(*block);
    set_alloc_block_flag(new_block, IS_ALLOCATED_FLAG, false);

    // transfer the last block flag over if the current block was the last one
    // on this page
    if (is_last_block(block)) {
        set_alloc_block_flag(new_block, IS_LAST_BLOCK_FLAG, true);
    }
    
    block->size = split_size;
    set_alloc_block_flag(block, IS_LAST_BLOCK_FLAG, false);
    set_alloc_block_flag(block, IS_ALLOCATED_FLAG, true);

    add_to_alloc_list_(free_list, new_block);
    add_to_alloc_list_(allocated_list, block);
    return new_block;
}
