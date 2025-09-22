#pragma once

#include <stddef.h>

#include "platform_specific.h"

PUBLIC_SYMBOL void* malloc(size_t size);
PUBLIC_SYMBOL void* calloc(size_t n, size_t size);
PUBLIC_SYMBOL void* realloc(void* ptr, size_t size);
PUBLIC_SYMBOL void free(void* ptr);

PUBLIC_SYMBOL void show_alloc_mem(void);

#ifdef FT_MALLOC_BONUS

/// @brief finds the number of usable bytes a block that
///        was allocated by malloc (or related)
/// @param ptr the block of memory allocated through malloc
/// @return the usable size or 0 if ptr is NULL or not
///         a pointer allocated through malloc (or related)
PUBLIC_SYMBOL size_t malloc_usable_size(void* ptr);

/// @brief Scans all blocks on the heap for overflow.
///        If a corrupted section is found the info
///        is printed to stderr
PUBLIC_SYMBOL void check_heap(void);

/// @brief prints a detailed overview of the heap to stdout
PUBLIC_SYMBOL void show_alloc_mem_ex(void);

#endif // FT_MALLOC_BONUS
