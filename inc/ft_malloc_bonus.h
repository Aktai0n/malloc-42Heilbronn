#pragma once

#include <stddef.h>

/// @brief finds the number of usable bytes a block that
///        was allocated by malloc (or related)
/// @param ptr the block of memory allocated through malloc
/// @return the usable size or 0 if ptr is NULL or not
///         a pointer allocated through malloc (or related)
size_t malloc_usable_size(void* ptr);
