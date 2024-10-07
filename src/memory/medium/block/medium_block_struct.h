#pragma once

#include <stddef.h>

#include "../../block_base/block_base.h"

typedef struct s_medium_block {
    t_block_base prev;
    t_block_base curr;
    struct s_medium_block* next_ptr;
    struct s_medium_block* prev_ptr;
} t_medium_block;
