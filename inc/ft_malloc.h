#ifndef FT_MALLOC_H
#define FT_MALLOC_H


#include <sys/mman.h>
#include <stdint.h>
#include <unistd.h>

#include "ft_malloc_internal.h"

#include "platform_specific.h"



PUBLIC_SYMBOL void* malloc(size_t size);
PUBLIC_SYMBOL void* calloc(size_t n, size_t size);
PUBLIC_SYMBOL void* realloc(void* ptr, size_t size);
PUBLIC_SYMBOL void free(void* ptr);

PUBLIC_SYMBOL void show_alloc_mem(void);

// BONUS
void show_alloc_mem_ex(void);


#endif // FT_MALLOC_H
