
#include "alloc_block.h"

#include "../../libft/libft.h"

static bool can_be_merged_(t_alloc_block* next) {
    return next != NULL && !is_allocated(next);
}

bool merge_alloc_block(t_alloc_block* block, t_alloc_block** free_list) {
    t_alloc_block* next = get_next_block_in_memory(block);

    if (!can_be_merged_(next)) {
        return false;
    }
    if (!delete_from_alloc_list(free_list, next)) {
        return false;
    }

    // increase the capacity of this block
    size_t new_size = get_alloc_size(block) + get_alloc_size(next) + sizeof(*next);
    set_alloc_size(block, new_size);
            ft_printf("is allocated: %d\n", is_allocated(block));

    if (is_last_block(next)) {
        set_alloc_block_flag(block, IS_LAST_BLOCK_FLAG, true);
    }
    next->size = 0;
    return true;
}
