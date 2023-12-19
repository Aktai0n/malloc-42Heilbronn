#pragma once

#include <stddef.h>
#include <stdbool.h>

void* allocate_memory(size_t requested_block_size);

void* reallocate_memory(void* ptr, size_t size);

bool release_memory(void* ptr);

/// @brief finds the number of usable bytes a block that
///        was allocated by malloc (or related)
/// @param ptr the block of memory allocated through malloc
/// @return the usable size or 0 if ptr is not a pointer
///         allocated through malloc (or related)
size_t available_memory_size(void* ptr);