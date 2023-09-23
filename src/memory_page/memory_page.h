#pragma once

#include <stdbool.h>
#include <unistd.h>
#include <stdint.h>

#include "../alloc_block/alloc_block.h"

enum e_memory_page {
    TINY_PAGE = 1,
    SMALL_PAGE = 2,
    LARGE_PAGE = 3
};

typedef struct s_memory_page {
    t_alloc_block* allocated_list;
    t_alloc_block* free_list;
    struct s_memory_page* next;
    size_t size;
    enum e_memory_page type;
} t_memory_page;




bool call_munmap(t_memory_page* page);
void* call_mmap(size_t size, int additional_flags);

t_memory_page* init_memory_page(
    size_t size,
    enum e_memory_page type,
    int additional_mmap_flags
);