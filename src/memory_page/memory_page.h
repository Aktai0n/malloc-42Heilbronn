#pragma once

#include <stdbool.h>
#include <unistd.h>

#include "../alloc/alloc_block.h"

enum e_memory_page {
    TINY_PAGE = 1,
    SMALL_PAGE = 2,
    LARGE_PAGE = 3
};

typedef struct s_memory_page {
    enum e_memory_page type;
    size_t size;
    t_alloc_block* allocated_list;
    t_alloc_block* free_list;
    struct s_memory_page* next;
} t_memory_page;




bool call_munmap(t_memory_page* page);
void* call_mmap(size_t size, int additional_flags);