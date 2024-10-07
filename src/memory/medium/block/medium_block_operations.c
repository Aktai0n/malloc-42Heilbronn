
#include "medium_block.h"

static void set_next_block_flag_(
    t_medium_block* block,
    size_t flag,
    bool state
) {
    t_medium_block* next = get_next_medium_block(block);
    if (next != NULL) {
        set_block_flag(&next->prev, flag, state);
    }
}

t_medium_block* allocate_medium_block(
    const size_t size,
    t_medium_block** free_list,
    t_medium_block** allocated_list
) {
    t_medium_block* block = find_medium_block(*free_list, size);
    if (block == NULL) {
        return NULL;
    }

    split_medium_block(block, size, free_list);

    if (delete_from_medium_block_list(free_list, block) == NULL) {
        return NULL;
    }
    set_block_flag(&block->curr, IS_ALLOCATED_FLAG, true);
    set_next_block_flag_(block, IS_ALLOCATED_FLAG, true);
    add_to_medium_block_list(allocated_list, block);
    return block;
}

bool deallocate_medium_block(
    t_medium_block** block,
    t_medium_block** free_list,
    t_medium_block** allocated_list
) {
    if (delete_from_medium_block_list(allocated_list, *block) == NULL) {
        return false;
    }
    merge_medium_block(block, true, free_list);
    set_block_flag(&(*block)->curr, IS_ALLOCATED_FLAG, false);
    set_next_block_flag_(*block, IS_ALLOCATED_FLAG, false);
    add_to_medium_block_list(free_list, *block);
    return true;
}

t_medium_block* reallocate_medium_block(
    t_medium_block* block,
    const size_t size,
    t_medium_block** free_list,
    t_medium_block** allocated_list
) {
    if (get_block_size(block->curr) > size) {
        return block;
    }

    // no backward merging allowed because the original ptr
    // needs to stay valid
    if (merge_medium_block(&block, false, free_list)) {
        if (get_block_size(block->curr) >= size) {
            if (is_last_block(block->curr)) {
                split_medium_block(block, size, free_list);
            }
            return block;
        }
    }

    t_medium_block* new_block = allocate_medium_block(
        size,
        free_list,
        allocated_list
    );
    if (new_block == NULL) {
        return NULL;
    }
    copy_medium_block_data(block, new_block);
    deallocate_medium_block(&block, free_list, allocated_list);
    return new_block;
}
