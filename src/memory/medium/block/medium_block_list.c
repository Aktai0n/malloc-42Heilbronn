
#include "medium_block.h"

t_medium_block* get_next_medium_block(t_medium_block* block) {
    if (is_last_block(block->curr)) {
        return NULL;
    }
    return (t_medium_block*)(
        (uintptr_t)block + sizeof(*block) + get_block_size(block->curr)
    );
}

t_medium_block* get_prev_medium_block(t_medium_block* block) {
    if (is_last_block(block->prev)) {
        return NULL;
    }
    return (t_medium_block*)(
        (uintptr_t)block - (sizeof(*block) + get_block_size(block->prev))
    );
}

t_medium_block* find_in_medium_block_list(const void* ptr, t_medium_block* list) {
    while (list != NULL) {
        const void* data = get_medium_block_data(list);
        if (data == ptr) {
            return list;
        }
        list = list->next_ptr;
    }
    return NULL;
}

t_medium_block* find_free_medium_block(
    t_medium_block* list,
    const size_t size
) {
    t_medium_block* best_fit = NULL;
    while (list != NULL) {
        const size_t block_size = get_block_size(list->curr);
        if (block_size == size) {
            return list;
        } else if (block_size > size &&
            (best_fit == NULL ||
            block_size - size < get_block_size(best_fit->curr) - size)
        ) {
            best_fit = list;
        }
        list = list->next_ptr;
    }
    return best_fit;
}

void add_to_medium_block_list(
    t_medium_block** list,
    t_medium_block* to_add
) {
    if (*list != NULL) {
        to_add->next_ptr = *list;
        (*list)->prev_ptr = to_add;
    }
    *list = to_add;
    to_add->prev_ptr = NULL;
}

t_medium_block* delete_from_medium_block_list(
    t_medium_block** list,
    t_medium_block* to_remove
) {
    if (*list == to_remove) {
        *list = to_remove->next_ptr;
    }
    t_medium_block* prev = to_remove->prev_ptr;
    t_medium_block* next = to_remove->next_ptr;
    if (prev != NULL) {
        prev->next_ptr = next;
    }
    if (next != NULL) {
        next->prev_ptr = prev;
    }
    to_remove->next_ptr = NULL;
    to_remove->prev_ptr = NULL;
    return to_remove;
}

bool medium_block_is_corrupted(t_medium_block* block) {
    t_medium_block* next = get_next_medium_block(block);
    if (next == NULL) {
        return false;
    }
    return get_prev_medium_block(next) != block;
}
