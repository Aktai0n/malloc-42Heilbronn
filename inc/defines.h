#ifndef DEFINES_H
#define DEFINES_H

#include <stddef.h>
#include <unistd.h>

/*
** MALLOC_ALIGNMENT controls the minimal amount of user overhead the 
** implementation produces.
** E.g. With a MALLOC_ALIGNMENT of 16, if you allocate 1 byte
** you are actually getting 16 bytes back
** MALLOC_ALIGNMENT has to be at least 16 bytes and a power of 2
*/
#ifndef FT_MALLOC_ALIGNMENT
#define FT_MALLOC_ALIGNMENT (2 * sizeof(size_t))
#else
#if ((FT_MALLOC_ALIGNMENT & ~0x8) != 0 || FT_MALLOC_ALIGNMENT < 16)
#error "FT_MALLOC_ALIGNMENT malformed!"
#endif
#endif // FT_MALLOC_ALIGNMENT

/// @brief rounds the given size up to a multiple of FT_MALLOC_ALIGNMENT.
///        If size is 0 the aligned size is also 0
/// @param size an unsigned integer to be rounded up to the alignment
/// @return the rounded up size
#define ALIGN_ALLOC_SIZE(size) (size_t)(((size) + FT_MALLOC_ALIGNMENT - 1) & \
                                        ~(FT_MALLOC_ALIGNMENT - 1))

inline void align_allocation_size(size_t* size) {
    // allocations of size 0 result in a block of size 16
    if (*size == 0) {
        *size = FT_MALLOC_ALIGNMENT;
        return;
    }
    const size_t remainder = *size % FT_MALLOC_ALIGNMENT;
    *size += (remainder == 0) ? 0 : (FT_MALLOC_ALIGNMENT - remainder);
}

/*
** Enable FT_MALLOC_USE_LOCKS to make the alloc family of functions thread safe
*/
#ifdef FT_MALLOC_BONUS
#define FT_MALLOC_USE_LOCKS 1
#endif

#ifndef TINY_PAGE_SIZE
#define TINY_PAGE_SIZE ((size_t)getpagesize() * 16UL)
#endif

#ifndef SMALL_PAGE_SIZE
#define SMALL_PAGE_SIZE ((size_t)getpagesize() * 128UL)
#endif

#ifndef TINY_ALLOC_BLOCK_SIZE
#define TINY_ALLOC_BLOCK_SIZE ((size_t)(TINY_PAGE_SIZE / 128))
#endif

#ifndef SMALL_ALLOC_BLOCK_SIZE
#define SMALL_ALLOC_BLOCK_SIZE ((size_t)(SMALL_PAGE_SIZE / 128))
#endif

#ifdef FT_MALLOC_USE_LOCKS
#define FT_MALLOC_ACQUIRE_LOCK(mutex) pthread_mutex_lock(mutex)
#define FT_MALLOC_RELEASE_LOCK(mutex) pthread_mutex_unlock(mutex)
#else
#define FT_MALLOC_ACQUIRE_LOCK(mutex) (void)mutex
#define FT_MALLOC_RELEASE_LOCK(mutex) (void)mutex
#endif

#endif // DEFINES_H
