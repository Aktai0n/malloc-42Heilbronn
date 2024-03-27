#pragma once

#include <stddef.h>

#include "../block/small_block_struct.h"

typedef struct s_small_page {
    size_t size;
    t_small_block* block_list;
    struct s_small_page* prev;
    struct s_small_page* next;
} t_small_page;



void add_to_small_page_list(t_small_page** list, t_small_page* new_page);

t_small_page* delete_from_small_page_list(
    t_small_page** list,
    t_small_page* to_remove
);

t_small_page* find_in_small_page_list(
    const void* ptr,
    t_small_page* list
);