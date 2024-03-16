#pragma once

#include <stddef.h>

/// @brief Specialized implementation for bzero which assumes that
///        start and end are 8 byte aligned
/// @param start the beginning of the memory region to be zeroed
/// @param end the end of the memory region to be zeroed
void ft_malloc_bzero(void* start, void* end);

/// @brief Specialized implementation for memcpy which assumes that
///        src and dest are 8 byte aligned and size is a multiple of 8
/// @param src the beginning of the memory region to be copied
/// @param dst the beginning of the memory region that the data will be
///            copied into
/// @param size the number of bytes to be copied
void ft_malloc_memcpy(void* src, void* dst, size_t size);
