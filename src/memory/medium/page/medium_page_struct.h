#pragma once

#include <stddef.h>

#include "../block/medium_block_struct.h"

typedef struct s_medium_page {
    size_t size;
    t_medium_block* block_list;
    struct s_medium_page* next;
    struct s_medium_page* prev;
} t_medium_page;
