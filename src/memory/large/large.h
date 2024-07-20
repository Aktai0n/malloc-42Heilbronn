#pragma once

#include <stdbool.h>

#include "page/large_page_struct.h"


t_large_page* allocate_large(size_t size, t_large_page** pages);

t_large_page* reallocate_large(
    t_large_page* page,
    const size_t size,
    t_large_page** pages
);

bool deallocate_large(t_large_page* page, t_large_page** pages);
