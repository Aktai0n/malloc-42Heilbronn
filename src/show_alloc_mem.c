
#include "libft.h"
#include "ft_malloc_internal.h"
#include "memory/small/small.h"
#include "memory/small/block/small_block.h"
#include "memory/medium/medium.h"
#include "memory/medium/block/medium_block.h"
#include "memory/large/page/large_page.h"

static void print_block_(int fd, const void* start, const void* end, size_t size) {
    ft_dprintf(fd, "%p - %p : %u bytes\n", start, end, size);
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

static size_t print_small_(int fd, const t_small_page* pages) {
    size_t total = 0;
    const t_small_page* page = get_next_small_page_(NULL, pages);
    while (page != NULL) {
        ft_dprintf(fd, "TINY : %p\n", page);
        t_small_block* block = page->block_list;
        while (block != NULL) {
            if (is_allocated(block->curr)) {
                size_t size = get_block_size(block->curr);
                print_block_(
                    fd,
                    get_small_block_data(block),
                    get_small_block_data_end(block),
                    size
                );
                total += size;
            }
            block = get_next_small_block(block);
        }
        page = get_next_small_page_(page, pages);
    }
    return total;
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

static t_medium_block* get_medium_block_list_start_(const t_medium_page* page) {
    if (page->allocated_list == NULL) {
        return NULL;
    }
    return (t_medium_block*)((uintptr_t)page + sizeof(*page));
}

static size_t print_medium_(int fd, const t_medium_page* pages) {
    size_t total = 0;
    const t_medium_page* page = get_next_medium_page_(NULL, pages);
    while (page != NULL) {
        ft_dprintf(fd, "SMALL: %p\n", page);
        t_medium_block* block = get_medium_block_list_start_(page);
        while (block != NULL) {
            if (is_allocated(block->curr)) {
                size_t size = get_block_size(block->curr);
                print_block_(
                    fd,
                    get_medium_block_data(block),
                    get_medium_block_data_end(block),
                    size
                );
                total += size;
            }
            block = get_next_medium_block(block);
        }
        page = get_next_medium_page_(page, pages);
    }
    return total;
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

static size_t print_large_(int fd, t_large_page* pages) {
    size_t total = 0;
    t_large_page* page = get_next_large_page_(NULL, pages);
    while (page != NULL) {
        ft_dprintf(fd, "LARGE: %p\n", page);
        size_t size = get_large_page_data_size(page);
        print_block_(
            fd,
            get_large_page_data(page),
            get_large_page_end(page),
            size
        );
        total += size;
        page = get_next_large_page_(page, pages);
    }
    return total;
}

void show_alloc_mem(void) {
    struct s_heap* heap = &g_heap;
    int fd = STDOUT_FILENO;
    size_t total = 0;

    total += print_small_(fd, heap->small_pages);
    total += print_medium_(fd, heap->medium_pages);
    total += print_large_(fd, heap->large_pages);
    ft_dprintf(fd, "Total: %u bytes\n", total);
}
