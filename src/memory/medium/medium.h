#pragma once

#include "block/medium_block_struct.h"
#include "page/medium_page_struct.h"


t_medium_block* allocate_medium(
    const size_t size,
    const bool set_zero,
    t_medium_page** pages
);
t_medium_block* reallocate_medium(
    t_medium_block* block,
    const size_t size,
    t_medium_page* page,
    t_medium_page** pages
);
bool deallocate_medium(
    t_medium_block* block,
    t_medium_page* page,
    t_medium_page** pages
);
