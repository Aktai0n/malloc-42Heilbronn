
#include <stdio.h>

#include "ft_malloc.h"
#include "memory_page/memory_page.h"
#include "alloc_block/alloc_block.h"

void print_memory_page(const t_memory_page* page) {
    if (page->type == TINY_PAGE) {
        printf("TINY");
    } else if (page->type == SMALL_PAGE) {
        printf("SMALL");
    } else if (page->type == LARGE_PAGE) {
        printf("LARGE");
    }
    printf(": %p\n", (void*)page);
    for (
        const t_alloc_block* block = page->allocated_list;
        block != NULL;
        block = get_next_alloc(block)
    ) {
        const size_t start = (size_t)(block) + sizeof(*block); 
        const size_t end = start + block->size;
        printf("%lX - %lX : %zu bytes\n", start, end, block->size);
    }
}

void show_alloc_mem(void) {
}