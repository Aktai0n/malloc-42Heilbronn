#pragma once

#include <stddef.h>
#include <stdbool.h>

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

/// @brief Validates that the given address is inside the memory region
/// @param ptr The address to validate
/// @param region_start The address of the memory regions beginning
/// @param region_size The size of the memory region
/// @return True if the address is inside the region or
///         False if not
bool ft_is_in_region(
    const void* ptr,
    const void* region_start,
    size_t region_size
);

/// @brief Writes `size` number of bytes pointed to by `ptr`
///        in hexadecimal to the file descriptor `fd`
/// @param fd The file descriptor to write to. E.g. `STDOUT_FILENO`
/// @param ptr A pointer to the memory that should be displayed
/// @param size The number of bytes that should be written
void ft_malloc_print_bytes(int fd, void* ptr, size_t size);
