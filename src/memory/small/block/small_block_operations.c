
#include "small_block.h"
#include "utils.h"

static void set_next_block_flag_(
    t_small_block* block,
    size_t flag,
    bool state
) {
    t_small_block* next = get_next_small_block(block);
    if (next != NULL) {
        set_block_flag(&next->prev, flag, state);
    }
}

t_small_block* allocate_small_block(
    const size_t size,
    t_small_block* list
) {
    t_small_block* block = find_free_small_block(list, size);
    if (block == NULL) {
        return NULL;
    }

    split_small_block(block, size);

    set_block_flag(&block->curr, IS_ALLOCATED_FLAG, true);
    set_next_block_flag_(block, IS_ALLOCATED_FLAG, true);
    return block;
}

void deallocate_small_block(t_small_block** block) {
    merge_small_block(block, true);
    set_block_flag(&(*block)->curr, IS_ALLOCATED_FLAG, false);
    set_next_block_flag_(*block, IS_ALLOCATED_FLAG, false);
}

t_small_block* reallocate_small_block(
    t_small_block* block,
    const size_t size,
    t_small_block* list
) {
    if (get_block_size(block->curr) >= size) {
        return block;
    }

    // no backward merging allowed because the original ptr
    // needs to stay valid
    if (merge_small_block(&block, false)) {
        if (get_block_size(block->curr) >= size) {
            if (is_last_block(block->curr)) {
                split_small_block(block, size);
            }
            return block;
        }
    }

    t_small_block* new_block = allocate_small_block(size, list);
    if (new_block == NULL) {
        return NULL;
    }
    copy_small_block_data(block, new_block);
    deallocate_small_block(&block);
    return new_block;
}
