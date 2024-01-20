#pragma once

#include <stddef.h>

typedef struct s_alloc_block {
    struct s_alloc_block* next;
    size_t size;
} t_alloc_block;
