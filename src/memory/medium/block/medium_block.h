#pragma once

#include <stddef.h>

#include "medium_block_struct.h"
#include "utils.h"

t_medium_block* get_next_medium_block(t_medium_block* block);

t_medium_block* get_prev_medium_block(t_medium_block* block);

t_medium_block* find_medium_block(
    t_medium_block* list,
    const size_t size
);

void add_to_medium_block_list(
    t_medium_block** list,
    t_medium_block* to_add
);

t_medium_block* delete_from_medium_block_list(
    t_medium_block** list,
    t_medium_block* to_remove
);

bool medium_block_is_corrupted(t_medium_block* block);

inline void* get_medium_block_data(t_medium_block* block) {
    return (void*)((size_t)block + sizeof(*block));
}

inline t_medium_block* get_medium_block(void* ptr) {
    return (t_medium_block*)((size_t)ptr - sizeof(t_medium_block));
}

inline void copy_medium_block_data(
    t_medium_block* src,
    t_medium_block* dst
) {
    ft_malloc_memcpy(
        get_medium_block_data(src),
        get_medium_block_data(dst),
        get_block_size(src->curr)
    );
}

t_medium_block* allocate_medium_block(
    const size_t size,
    t_medium_block** free_list,
    t_medium_block** allocated_list
);

bool deallocate_medium_block(
    t_medium_block* block,
    t_medium_block** free_list,
    t_medium_block** allocated_list
);

t_medium_block* reallocate_medium_block(
    t_medium_block* block,
    const size_t size,
    t_medium_block** free_list,
    t_medium_block** allocated_list
);