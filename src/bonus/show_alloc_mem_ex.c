#include "ft_malloc_internal.h"
#include "libft.h"
#include "colors.h"
#include "../memory/small/page/small_page.h"
#include "../memory/small/block/small_block.h"
#include "../memory/medium/page/medium_page.h"
#include "../memory/medium/block/medium_block.h"
#include "../memory/large/page/large_page.h"

#include <stdint.h>

static void print_bytes_(int fd, void* ptr, size_t size, const char* color) {
#ifdef FT_MALLOC_BONUS
    if (!isatty(fd)) {
        color = NULL;
    }
#endif
    const size_t row_len = FT_MALLOC_ALIGNMENT;
    size_t i = 0;
    for (;(i + row_len) < size; i += row_len) {
        void* pos = (void*)((uintptr_t)ptr + i);
        ft_dprintf(fd, "%p ", pos);
        if (color != NULL) {
            ft_putstr_fd(color, fd);
        }
        ft_malloc_print_bytes(fd, pos, row_len);
        if (color != NULL) {
            ft_putstr_fd(RESET_COLOR, fd);
        }
        ft_putstr_fd("\n", fd);
    }
    void* pos = (void*)((uintptr_t)ptr + i);
    ft_dprintf(fd, "%p ", pos);
    if (color != NULL) {
        ft_putstr_fd(color, fd);
    }
    ft_malloc_print_bytes(fd, pos, size - i);
    if (color != NULL) {
        ft_putstr_fd(RESET_COLOR, fd);
    }
    ft_putstr_fd("\n", fd);
}

static void print_small_block_(int fd, t_small_block* block) {
    print_bytes_(fd, block, sizeof(*block), BOLD_INTENSE_CYAN_COLOR);
    void* data = get_small_block_data(block);
    size_t size = get_block_size(block->curr);
    const char* color = NULL;
    if (is_allocated(block->curr)) {
        color = BOLD_INTENSE_RED_COLOR;
    } else {
        color = BOLD_INTENSE_GREEN_COLOR;
    }
    print_bytes_(fd, data, size, color);
}

static void print_small_(int fd, t_small_page* pages) {
    for (t_small_page* page = pages; page != NULL; page = page->next) {
        print_bytes_(fd, page, sizeof(*page), BOLD_INTENSE_BLUE_COLOR);
        for (t_small_block* block = page->block_list; block != NULL;) {
            print_small_block_(fd, block);
            block = get_next_small_block(block);
        }
    }
}

static void print_medium_block_(int fd, t_medium_block* block) {
    print_bytes_(fd, block, sizeof(*block), BOLD_INTENSE_CYAN_COLOR);
    void* data = get_medium_block_data(block);
    size_t size = get_block_size(block->curr);
    const char* color = NULL;
    if (is_allocated(block->curr)) {
        color = BOLD_INTENSE_RED_COLOR;
    } else {
        color = BOLD_INTENSE_GREEN_COLOR;
    }
    print_bytes_(fd, data, size, color);
}

static t_medium_block* get_first_medium_block_(t_medium_page* page) {
    if (page->allocated_list == NULL && page->free_list == NULL) {
        return NULL;
    }
    t_medium_block* block = page->allocated_list;
    uintptr_t first = UINTPTR_MAX;
    while (block != NULL) {
        if ((uintptr_t)block < first) {
            first = (uintptr_t)block;
        }
        block = block->next_ptr;
    }
    block = page->free_list;
    while (block != NULL) {
        if ((uintptr_t)block < first) {
            first = (uintptr_t)block;
        }
        block = block->next_ptr;
    }
    return (t_medium_block*)first;
}

static void print_medium_(int fd, t_medium_page* pages) {
    for (t_medium_page* page = pages; page != NULL; page = page->next) {
        print_bytes_(fd, page, sizeof(*page), BOLD_INTENSE_BLUE_COLOR);
        t_medium_block* block = get_first_medium_block_(page);
        while (block != NULL) {
            print_medium_block_(fd, block);
            block = get_next_medium_block(block);
        }
    }
}

static void print_large_(int fd, t_large_page* pages) {
    for (t_large_page* page = pages; page != NULL; page = page->next) {
        print_bytes_(fd, page, sizeof(*page), BOLD_INTENSE_CYAN_COLOR);
        void* data = get_large_page_data(page);
        size_t size = get_large_page_data_size(page);
        print_bytes_(fd, data, size, BOLD_INTENSE_RED_COLOR);
        t_large_page_end* end = get_large_page_end(page);
        print_bytes_(fd, end, sizeof(*end), BOLD_INTENSE_CYAN_COLOR);
    }
}

void show_alloc_mem_ex(void) {
    struct s_heap* heap = &g_heap;
    int fd = STDOUT_FILENO;

    print_small_(fd, heap->small_pages);
    print_medium_(fd, heap->medium_pages);
    print_large_(fd, heap->large_pages);
}
