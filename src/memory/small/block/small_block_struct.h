#pragma once

#include "../../block_base/block_base.h"

typedef struct s_small_block {
    t_block_base prev;
    t_block_base curr;
} t_small_block;
