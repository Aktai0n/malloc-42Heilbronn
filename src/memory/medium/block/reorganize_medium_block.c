
#include "medium_block.h"
#include "defines.h"

static t_medium_block* merge_with_next_(
    t_medium_block* block,
    t_medium_block* next
) {
    // increase the capacity of block
    size_t block_size = get_block_size(block->curr);
    size_t next_size = get_block_size(next->curr);
    size_t new_size = block_size + next_size + sizeof(*next);
    set_block_size(&block->curr, new_size);

    // transfer flags
    if (is_last_block(next->curr)) {
        set_block_flag(&block->curr, IS_LAST_BLOCK_FLAG, true);
    } else {
        // set prev of the block after next
        // to the current block
        t_medium_block* next_next = get_next_medium_block(next);
        next_next->prev = block->curr;
    }

    // destroy next
    next->curr.size = 0;
    next->prev.size = 0;
    return block;
}

bool merge_medium_block(
    t_medium_block** block,
    bool merge_backwards,
    t_medium_block** free_list
) {
    bool ret = false;
    t_medium_block* next = get_next_medium_block(*block);
    while (next != NULL && !is_allocated(next->curr)) {
        if (delete_from_medium_block_list(free_list, next) == NULL) {
            break;
        }
        ret = true;
        merge_with_next_(*block, next);
        next = get_next_medium_block(*block);
    }
    if (merge_backwards) {
        t_medium_block* prev = get_prev_medium_block(*block);
        while (prev != NULL && !is_allocated(prev->curr)) {
            if (delete_from_medium_block_list(free_list, prev) == NULL) {
                break;
            }
            ret = true;
            *block = merge_with_next_(prev, *block);
            prev = get_prev_medium_block(*block);
        }
    }
    return ret;
}

static bool can_be_splitted_(
    const size_t block_size,
    const size_t split_size
) {
    return block_size >=
        split_size + TINY_ALLOC_BLOCK_SIZE + sizeof(t_medium_block);
}

bool split_medium_block(
    t_medium_block* block,
    const size_t split_size,
    t_medium_block** free_list
) {
    const size_t block_size = get_block_size(block->curr);
    if (!can_be_splitted_(block_size, split_size)) {
        return false;
    }

    // create and initialize new block
    t_medium_block* new_block = (t_medium_block*)(
        (uintptr_t)get_medium_block_data(block) + split_size
    );
    size_t new_block_size = block_size - (split_size + sizeof(*block));
    set_block_size(&new_block->curr, new_block_size);

    // transfer flags
    set_block_flag(&new_block->curr, IS_ALLOCATED_FLAG, false);
    if (is_last_block(block->curr)) {
        set_block_flag(&new_block->curr, IS_LAST_BLOCK_FLAG, true);
        set_block_flag(&block->curr, IS_LAST_BLOCK_FLAG, false);
    } else {
        set_block_flag(&new_block->curr, IS_LAST_BLOCK_FLAG, false);
    }

    // shrink original block
    set_block_size(&block->curr, split_size);

    // add new block to lists
    new_block->prev = block->curr;
    add_to_medium_block_list(free_list, new_block);
    return true;
}
