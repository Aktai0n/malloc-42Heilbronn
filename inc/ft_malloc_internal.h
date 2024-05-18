#pragma once

#include <pthread.h>
#include "platform_specific.h"

#if 1

typedef struct s_small_page t_small_page;
typedef struct s_medium_page t_medium_page;
typedef struct s_large_page t_large_page;

struct s_heap {
    t_small_page* tiny_pages;
    t_medium_page* small_pages;
    t_large_page* large_pages;
};

#else

typedef struct s_memory_page t_memory_page;

struct s_heap {
    t_memory_page* tiny_pages;
    t_memory_page* small_pages;
    t_memory_page* large_pages;
};

#endif

// both are defined in malloc.c
extern struct s_heap g_heap;
extern pthread_mutex_t g_alloc_mutex;
