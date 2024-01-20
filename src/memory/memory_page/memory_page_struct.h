#pragma once

#include <stddef.h>

#include "../alloc_block/alloc_block_struct.h"
#include "memory_page_enum.h"

typedef struct s_memory_page {
    t_alloc_block* allocated_list;
    t_alloc_block* free_list;
    struct s_memory_page* next;
    size_t size;
    enum e_memory_page type;
} t_memory_page;