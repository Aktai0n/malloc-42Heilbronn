#pragma once

#include <stddef.h>
#include <stdbool.h>

/// @brief Allocates a block of memory with the requested size
/// @param requested_block_size The size to be allocated
/// @param set_zero If true all the memory in the block
///                 is set to zero
/// @return A pointer to the memory block or
///         NULL if no block can be found or
///         created to fit the requested size
void* allocate_memory(size_t requested_block_size, bool set_zero);

void* reallocate_memory(void* ptr, size_t size);

bool release_memory(void* ptr);

/// @brief finds the number of usable bytes a block that
///        was allocated by malloc (or related)
/// @param ptr the block of memory allocated through malloc
/// @return the usable size or 0 if ptr is not a pointer
///         allocated through malloc (or related)
size_t available_memory_size(void* ptr);