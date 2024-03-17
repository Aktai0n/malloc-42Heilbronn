#ifndef FT_MALLOC_H
#define FT_MALLOC_H


#include <sys/mman.h>
#include <stdint.h>
#include <unistd.h>

#include "ft_malloc_internal.h"


int ft_printf(const char* format, ...);







void* malloc(size_t size);
void* calloc(size_t n, size_t size);
void* realloc(void* ptr, size_t size);
void free(void* ptr);

void show_alloc_mem(void);

// BONUS
void show_alloc_mem_ex(void);


#endif // FT_MALLOC_H
