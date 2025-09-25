#pragma once

#include <stddef.h>

#include "../block/small_block_struct.h"

typedef struct s_small_page {
    size_t size;
    t_small_block* block_list;
    struct s_small_page* prev;
    struct s_small_page* next;
} t_small_page;
