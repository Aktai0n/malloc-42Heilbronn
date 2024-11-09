#pragma once

#include <pthread.h>
#include "platform_specific.h"
#include "defines.h"
#include "ft_malloc.h"

typedef struct s_small_page t_small_page;
typedef struct s_medium_page t_medium_page;
typedef struct s_large_page t_large_page;

struct s_heap {
    t_small_page* small_pages;
    t_medium_page* medium_pages;
    t_large_page* large_pages;
};

/// @brief Global heap. Tracks all mmaped pages.
///        Defined in `constructor_and_destructor.c`
extern struct s_heap g_heap;

/// @brief Global mutex to make allocations thread safe.
///        Defined in `constructor_and_destructor.c`
extern pthread_mutex_t g_alloc_mutex;
