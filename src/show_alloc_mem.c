


#if 1

#include "libft.h"
#include "ft_malloc_internal.h"
#include "memory/small/small.h"
#include "memory/small/block/small_block.h"
#include "memory/medium/medium.h"
#include "memory/medium/block/medium_block.h"
#include "memory/large/page/large_page.h"

static void print_page_(const void* start, size_t size) {
    const void* end = (const void*)((const uintptr_t)start + size);
    ft_printf("%p - %p : %u bytes\n", start, end, size);
}

static void print_block_(const void* start, const void* end, size_t size) {
    ft_printf("%p - %p : %u bytes\n", start, end, size);
}

static const t_small_page* get_next_small_page_(
    const t_small_page* current,
    const t_small_page* pages
) {
    const t_small_page* next = NULL;
    ptrdiff_t min_diff = PTRDIFF_MAX;
    for (const t_small_page* page = pages; page != NULL; page = page->next) {
        ptrdiff_t diff = (intptr_t)page - (intptr_t)current;
        if (diff > 0 && diff < min_diff) {
            min_diff = diff;
            next = page;
        }
    }
    return next;
}

static void print_small_(const t_small_page* pages) {
    const t_small_page* page = get_next_small_page_(NULL, pages);
    while (page != NULL) {
        ft_printf("TINY : %p\n", page);
        t_small_block* block = page->block_list;
        while (block != NULL) {
            if (is_allocated(block->curr)) {
                print_block_(
                    get_small_block_data(block),
                    get_small_block_data_end(block),
                    get_block_size(block->curr)
                );
            }
            block = get_next_small_block(block);
        }
        page = get_next_small_page_(page, pages);
    }
}

static const t_medium_page* get_next_medium_page_(
    const t_medium_page* current,
    const t_medium_page* pages
) {
    const t_medium_page* next = NULL;
    ptrdiff_t min_diff = PTRDIFF_MAX;
    for (const t_medium_page* page = pages; page != NULL; page = page->next) {
        ptrdiff_t diff = (intptr_t)page - (intptr_t)current;
        if (diff > 0 && diff < min_diff) {
            min_diff = diff;
            next = page;
        }
    }
    return next;
}

static void print_medium_(const t_medium_page* pages) {
    const t_medium_page* page = get_next_medium_page_(NULL, pages);
    while (page != NULL) {
        ft_printf("SMALL: %p\n", page);
        t_medium_block* block = page->allocated_list;
        while (block != NULL) {
            print_block_(
                get_medium_block_data(block),
                get_medium_block_data_end(block),
                get_block_size(block->curr)
            );
            block = block->next_ptr;
        }
        page = get_next_medium_page_(page, pages);
    }
}

static const t_large_page* get_next_large_page_(
    const t_large_page* current,
    const t_large_page* pages
) {
    const t_large_page* next = NULL;
    ptrdiff_t min_diff = PTRDIFF_MAX;
    for (const t_large_page* page = pages; page != NULL; page = page->next) {
        ptrdiff_t diff = (intptr_t)page - (intptr_t)current;
        if (diff > 0 && diff <= min_diff) {
            min_diff = diff;
            next = page;
        }
    }
    return next;
}

static void print_large_(const t_large_page* pages) {
    const t_large_page* page = get_next_large_page_(NULL, pages);
    while (page != NULL) {
        ft_printf("LARGE: %p\n", page);
        print_block_(
            get_large_page_data(page),
            get_large_page_end(page),
            get_large_page_data_size(page)
        );
        page = get_next_large_page_(page, pages);
    }
}

void show_alloc_mem(void) {
    struct s_heap* heap = &g_heap;
    print_small_(heap->tiny_pages);
    print_medium_(heap->small_pages);
    print_large_(heap->large_pages);
}


#else

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

#endif