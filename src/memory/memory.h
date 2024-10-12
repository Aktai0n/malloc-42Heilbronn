#pragma once

#include <stddef.h>
#include <stdbool.h>

/// @brief Allocates a block of memory with the requested size
/// @param size The size to be allocated
/// @param set_zero If true all the memory in the block
///                 is set to zero
/// @return A pointer to the memory block or
///         NULL if no block can be found or
///         created to fit the requested size
void* allocate_memory(size_t size, bool set_zero);

/// @brief Attempts to resize the memory block pointed to by `ptr`
///        to fit at least `size` bytes.
///        If the resizing attempt fails, a new block of sufficient
///        `size` is allocated, the contents are copied over and
///        the block is returned.
///        The old block is deallocated in this case
/// @param ptr A pointer to an allocated block of memory
/// @param size The minimal size the block should have
/// @return A pointer to the block of memory with at least `size`
///         bytes of capacity. If no new block has to be allocated
///         it's equivalent to `ptr`
void* reallocate_memory(void* ptr, size_t size);

/// @brief Attempts to free the given block of memory.
///        If the operation fails `errno` is set to:
///        - EINVAL: If ptr is not found on the heap
///        - EACCESS: If ptr is not allocated (double free)
/// @param ptr A pointer to an allocated block of memory
/// @return True if the operation was succesful and
///         False if not
bool release_memory(void* ptr);

/// @brief Finds the number of usable bytes a block that
///        was allocated by malloc (or related)
/// @param ptr A pointer to an allocated block of memory
/// @return The usable size or
///         0 if `ptr` is not found on the heap
size_t available_memory_size(void* ptr);


// ---------------------- abi_wrappers.c -----------------------

/// @brief Maps a memory page into the memory region of the program
/// @param size The amount of bytes the memory page should have
/// @param additional_flags Additionall flags to be passed to `mmap`.
///                         MAP_PRIVATE and MAP_ANONYMOUS are
///                         always set
/// @return A pointer to the memory page or
///         NULL if `mmap` fails
void* call_mmap(size_t size, int additional_flags);
