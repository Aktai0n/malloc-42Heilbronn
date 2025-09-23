#pragma once

#include <stddef.h>

#include "platform_specific.h"

/// @brief Attempts to allocate `size` bytes of memory from the operating system
///        and returns a pointer to the allocated memory
/// @param size The number of bytes to allocate
/// @return A pointer to the memory region that's at least `size` bytes big
///         or NULL if the allocation fails
PUBLIC_SYMBOL void* malloc(size_t size);

/// @brief Attempts to allocate a block of memory that's at least `n * size` bytes in size
///        from the operating system. The memory area is zeroed before it's returned
/// @param n The number of elements that should be allocated
/// @param size The size of each element
/// @return A pointer to the zeroed memory region that's at least `n * size` bytes big
///         or NULL if the allocation fails
PUBLIC_SYMBOL void* calloc(size_t n, size_t size);

/// @brief Attempts to change the size of the memory area pointed to by `ptr`
///        to fit at least `size` bytes. If there is not enough capacity in the memory area
///        realloc will allocate a new block, copy the data at `ptr` to the new block
///        and free the old block
/// @param ptr A pointer to an existing block of memory whose size should be changed
/// @param size The size that the memory block pointed to by `ptr` should minimaly hold
/// @return `ptr` if the size increase was successful,
///         a pointer to a new block of memory if the size of `ptr` couldn't be increased
///         or NULL if the allocation failed
PUBLIC_SYMBOL void* realloc(void* ptr, size_t size);

/// @brief Deallocates the memory block pointed to by `ptr`
/// @param ptr A pointer to the memory region that should be deallocated
PUBLIC_SYMBOL void free(void* ptr);

/// @brief Prints information about the allocated pages and blocks to stdout
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

/// @brief Prints a detailed overview of the heap to stdout
PUBLIC_SYMBOL void show_alloc_mem_ex(void);

#endif // FT_MALLOC_BONUS
