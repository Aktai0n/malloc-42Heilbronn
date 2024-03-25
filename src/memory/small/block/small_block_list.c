
#include "small_block.h"

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
