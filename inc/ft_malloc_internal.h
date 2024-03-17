#pragma once

#include <pthread.h>

typedef struct s_memory_page t_memory_page;

struct s_heap {
    t_memory_page* tiny_pages;
    t_memory_page* small_pages;
    t_memory_page* large_pages;
};

// both are defined in malloc.c
extern struct s_heap g_heap;
extern pthread_mutex_t g_alloc_mutex;
