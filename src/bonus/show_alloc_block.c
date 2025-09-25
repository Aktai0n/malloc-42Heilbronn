
#include <errno.h>

#include "ft_malloc_internal.h"
#include "libft.h"
#include "utils.h"
#include "colors.h"
#include "../memory/small/page/small_page.h"
#include "../memory/small/block/small_block.h"
#include "../memory/medium/page/medium_page.h"
#include "../memory/medium/block/medium_block.h"
#include "../memory/large/page/large_page.h"

static void print_small_(t_small_block* block) {
    void* data = get_small_block_data(block);
    size_t size = get_block_size(block->curr);
    ft_printf(
        "small block from %p to %p, usable size=%u, is allocated=%d, is corrupted=%d\n",
        (void*)block,
        get_small_block_data_end(block),
        size,
        is_allocated(block->curr),
        small_block_is_corrupted(block)
    );
    ft_putstr(BOLD_CYAN_COLOR);
    ft_malloc_print_bytes(STDOUT_FILENO, block, sizeof(*block));
    if (is_allocated(block->curr)) {
        ft_putstr(BOLD_RED_COLOR);
    } else {
        ft_putstr(BOLD_GREEN_COLOR);
    }
    ft_malloc_print_bytes(STDOUT_FILENO, data, size);
}

static void print_medium_(t_medium_block* block) {
    size_t size = get_block_size(block->curr);
    void* data = get_medium_block_data(block);
    ft_printf(
        "medium block from %p to %p, usable size=%u, is allocated=%d, is corrupted=%d\n",
        (void*)block,
        get_medium_block_data_end(block),
        size,
        is_allocated(block->curr),
        medium_block_is_corrupted(block)
    );
    ft_putstr(BOLD_CYAN_COLOR);
    ft_malloc_print_bytes(STDOUT_FILENO, block, sizeof(*block));
    if (is_allocated(block->curr)) {
        ft_putstr(BOLD_RED_COLOR);
    } else {
        ft_putstr(BOLD_GREEN_COLOR);
    }
    ft_malloc_print_bytes(STDOUT_FILENO, data, size);
}

static void print_large_(t_large_page* page, void* data) {
    size_t size = get_large_page_data_size(page);
    ft_printf(
        "large block from %p to %p, usable size=%u, is corrupted=%d\n",
        (void*)page,
        (void*)get_large_page_end(page),
        size,
        large_page_is_corrupted(page)
    );
    ft_putstr(BOLD_CYAN_COLOR);
    ft_malloc_print_bytes(STDOUT_FILENO, page, sizeof(*page));
    ft_putstr(BOLD_GREEN_COLOR);
    ft_malloc_print_bytes(STDOUT_FILENO, data, size);
}

static bool print_block_(void* ptr, struct s_heap* heap) {
    t_small_page* small_page = find_in_small_page_list(ptr, heap->small_pages);
    if (small_page != NULL) {
        t_small_block* block = find_in_small_block_list(ptr, small_page->block_list);
        if (block != NULL) {
            print_small_(block);
            return true;
        }
        return false;
    }
    t_medium_page* medium_page = find_in_medium_page_list(ptr, heap->medium_pages);
     if (medium_page != NULL) {
        t_medium_block* block = find_in_medium_block_list(ptr, medium_page->allocated_list);
        if (block != NULL) {
            print_medium_(block);
            return true;
        }
        block = find_in_medium_block_list(ptr, medium_page->free_list);
        if (block != NULL) {
            print_medium_(block);
            return true;
        }
        return false;
    }
    t_large_page* large_page = find_in_large_page_list(ptr, heap->large_pages);
    if (large_page != NULL) {
        print_large_(large_page, ptr);
        return true;
    }
    return false;
}

void show_alloc_block(void* ptr) {
    struct s_heap* heap = &g_heap;

    FT_MALLOC_ACQUIRE_LOCK(&g_alloc_mutex);
    if (!print_block_(ptr, heap)) {
        ft_dprintf(STDERR_FILENO, "ptr not found %p\n", ptr);
        errno = EINVAL;
    }
    FT_MALLOC_RELEASE_LOCK(&g_alloc_mutex);
}
