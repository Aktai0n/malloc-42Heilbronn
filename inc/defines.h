#ifndef DEFINES_H
#define DEFINES_H

#include <stddef.h>


/*
** MALLOC_ALIGNMENT controls the minimal amount of user overhead the 
** implementation produces.
** E.g. With a MALLOC_ALIGNMENT of 16, if you allocate 1 byte
** you are actually getting 16 bytes back
** MALLOC_ALIGNMENT has to be at least 16 bytes and a power of 2
*/
#ifndef MALLOC_ALIGNMENT
#define MALLOC_ALIGNMENT ((size_t)16U)
#else
#if ((MALLOC_ALIGNMENT % 8) != 0)
#error "MALLOC_ALIGNMENT malformed!"
#endif
#endif // MALLOC_ALIGNMENT

/*
** Enable USE_LOCKS to make the alloc family of functions thread safe
*/
#ifdef MALLOC_BONUS
#define USE_LOCKS 1
#endif





#endif // DEFINES_H