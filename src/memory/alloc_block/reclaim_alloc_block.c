

#include "alloc_block.h"

// bool free_alloc_block(
//     t_alloc_block* to_free,
//     t_alloc_block** free_list,
//     t_alloc_block** allocated_list
// ) {
//     if (!delete_from_alloc_list(allocated_list, to_free)) {
//         return false;
//     }
//     set_alloc_block_flag(to_free, IS_ALLOCATED_FLAG, false);

//     if (merge_alloc_block(to_free, free_list)) {
//         return true;
//     }
//     add_to_alloc_list(free_list, to_free);
//     return true;
// }

bool reclaim_alloc_block(
    t_alloc_block* block,
    t_alloc_block** free_list,
    t_alloc_block** allocated_list
) {
    // delete the block from the allocated list and add it to the free list
    if (delete_from_alloc_list(allocated_list, block) == NULL) {
        return false;
    }
    set_alloc_block_flag(block, IS_ALLOCATED_FLAG, false);
    add_to_alloc_list(free_list, block);

    // try to merge this block with the next in memory (avoid fragmentation)
    merge_alloc_block(block, free_list);
    return true;
}