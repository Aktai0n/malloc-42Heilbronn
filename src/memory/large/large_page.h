#pragma once

#include <stddef.h>
#include <stdbool.h>

#include "large_page_struct.h"

void add_to_large_page_list(t_large_page** list, t_large_page* new_page);

t_large_page* delete_from_large_page_list(
    t_large_page** list,
    t_large_page* to_remove
);