
#include "libft.h"
#include "ft_malloc_internal.h"
#include "memory/small/small.h"
#include "memory/small/block/small_block.h"
#include "memory/medium/medium.h"
#include "memory/medium/block/medium_block.h"
#include "memory/large/page/large_page.h"

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

static t_large_page* get_next_large_page_(
    t_large_page* current,
    t_large_page* pages
) {
    t_large_page* next = NULL;
    ptrdiff_t min_diff = PTRDIFF_MAX;
    for (t_large_page* page = pages; page != NULL; page = page->next) {
        ptrdiff_t diff = (intptr_t)page - (intptr_t)current;
        if (diff > 0 && diff <= min_diff) {
            min_diff = diff;
            next = page;
        }
    }
    return next;
}

static void print_large_(t_large_page* pages) {
    t_large_page* page = get_next_large_page_(NULL, pages);
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
    print_small_(heap->small_pages);
    print_medium_(heap->medium_pages);
    print_large_(heap->large_pages);
}
