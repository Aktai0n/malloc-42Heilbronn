
#include <stdbool.h>

#include "alloc_block.h"

t_alloc_block* init_alloc_block(
    const size_t size,
    t_alloc_block** free_list,
    t_alloc_block** allocated_list
) {
    t_alloc_block* block = find_alloc_block(*free_list, size);
    if (block == NULL) {
        return NULL;
    }

    // try to split the memory block when its size
    // is bigger than the requested size
    if (get_alloc_size(block) > size) {
        t_alloc_block* split_result = split_alloc_block(
            block,
            size,
            free_list,
            allocated_list
        );
        if (split_result != NULL) {
            return split_result;
        }
    }

    // transfer the memory block from the free_list to the allocated_list
    delete_from_alloc_list(free_list, block);
    set_alloc_block_flag(block, IS_ALLOCATED_FLAG, true);
    add_to_alloc_list(allocated_list, block);
    return block;
}
