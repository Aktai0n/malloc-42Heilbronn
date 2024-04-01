
#include "small_block.h"

t_small_block* get_next_small_block(t_small_block* block) {
    if (is_last_block(block->curr)) {
        return NULL;
    }
    return (t_small_block*)(
        (size_t)block + sizeof(*block) + get_block_size(block->curr)
    );
}

t_small_block* get_prev_small_block(t_small_block* block) {
    size_t prev_size = get_block_size(block->prev);
    if (prev_size == 0) {
        return NULL;
    }
    return (t_small_block*)(
        (size_t)block - (sizeof(*block) + prev_size)
    );
}

t_small_block* find_small_block(
    t_small_block* list,
    const size_t size
) {
    while (list != NULL) {
        if (!is_allocated(list->curr) &&
            get_block_size(list->curr) >= size
        ) {
            return list;
        }
        list = get_next_small_block(list);
    }
    return NULL;
}

bool small_block_is_corrupted(t_small_block* block) {
    t_small_block* next = get_next_small_block(block);
    if (next == NULL) {
        return false;
    }
    return get_prev_small_block(next) != block;
}
