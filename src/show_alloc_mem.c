
#include <stdio.h>

#include "ft_malloc.h"
#include "libft.h"
#include "memory/memory_page/memory_page.h"
#include "memory/alloc_block/alloc_block.h"

static size_t print_alloc_block_(t_alloc_block* block) {
    size_t size = get_alloc_size(block);
    void* start = get_alloc_data(block);
    void* end = (void*)((size_t)start + size);
    ft_printf("%p - %p : %u bytes\n", start, end, size);
    return size;
}

static size_t print_memory_page_(t_memory_page* page) {
    if (page->type == TINY_PAGE) {
        ft_printf("TINY : ");
    } else if (page->type == SMALL_PAGE) {
        ft_printf("SMALL : ");
    } else {
        ft_printf("LARGE : ");
    }
    void* start = (void*)page;
    void* end = (void*)((size_t)start + page->size);
    ft_printf("%p - %p\n", start, end);

    t_alloc_block* block = page->allocated_list;
    size_t total_alloc_size = 0;
    while (block != NULL) {
        total_alloc_size += print_alloc_block_(block);
        block = get_next_block_in_memory(block);
    }
    return total_alloc_size;
}

// static size_t print_memory_pages_(t_memory_page* pages) {
//     size_t total_alloc_size = 0;
//     for (t_memory_page* page = pages; page != NULL; page = page->next) {
//         total_alloc_size += print_memory_page_(page);
//     }
//     return total_alloc_size;
// }

static t_memory_page* find_next_page_(t_memory_page* prev) {
    t_memory_page* page = NULL;
    t_memory_page* next = NULL;
    ptrdiff_t min_diff = PTRDIFF_MAX;
    for (page = g_heap.tiny_pages; page != NULL; page = page->next) {
        ptrdiff_t diff = page - prev;
        if (diff > 0 && diff < min_diff) {
            next = page;
        }
    }
    for (page = g_heap.small_pages; page != NULL; page = page->next) {
        ptrdiff_t diff = page - prev;
        if (diff > 0 && diff < min_diff) {
            next = page;
        }
    }
    for (page = g_heap.large_pages; page != NULL; page = page->next) {
        ptrdiff_t diff = page - prev;
        if (diff > 0 && diff < min_diff) {
            next = page;
        }
    }
    return next;
}

// needs testing!
void show_alloc_mem(void) {
    size_t total_alloc_size = 0;
    t_memory_page* page = find_next_page_(NULL);
    ft_printf("page: %p\n", page);
    while (page != NULL) {
        total_alloc_size += print_memory_page_(page);
        page = find_next_page_(page);
    }
    ft_printf("Total : %u\n", total_alloc_size);
}
