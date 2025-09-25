#pragma once

#include <stddef.h>

typedef struct s_large_page {
    size_t size;
    struct s_large_page* next;
    struct s_large_page* prev;
} t_large_page;

typedef struct s_large_page_end {
    size_t size;
} t_large_page_end;
