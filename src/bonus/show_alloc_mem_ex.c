


#if 1

#include "ft_malloc_internal.h"
#include "libft.h"
#include "colors.h"
#include "../memory/small/page/small_page.h"
#include "../memory/small/block/small_block.h"
#include "../memory/medium/page/medium_page.h"
#include "../memory/medium/block/medium_block.h"
#include "../memory/large/page/large_page.h"

#include <stdint.h>

static void print_small_block_(int fd, t_small_block* block) {
    ft_putstr_fd(BOLD_INTENSE_CYAN_COLOR, fd);
    ft_malloc_print_bytes(fd, block, sizeof(*block));
    void* data = get_small_block_data(block);
    size_t size = get_block_size(block->curr);
    if (is_allocated(block->curr)) {
        ft_putstr_fd(BOLD_INTENSE_RED_COLOR, fd);
    } else {
        ft_putstr_fd(BOLD_INTENSE_GREEN_COLOR, fd);
    }
    ft_malloc_print_bytes(fd, data, size);
}

static void print_small_(int fd, t_small_page* pages) {
    for (t_small_page* page = pages; page != NULL; page = page->next) {
        ft_putstr_fd(BOLD_INTENSE_BLUE_COLOR, fd);
        ft_malloc_print_bytes(fd, page, sizeof(*page));
        for (t_small_block* block = page->block_list; block != NULL;) {
            print_small_block_(fd, block);
            block = get_next_small_block(block);
        }
    }
}

static void print_medium_block_(int fd, t_medium_block* block) {
    ft_putstr_fd(BOLD_INTENSE_CYAN_COLOR, fd);
    ft_malloc_print_bytes(fd, block, sizeof(*block));
    void* data = get_medium_block_data(block);
    size_t size = get_block_size(block->curr);
    if (is_allocated(block->curr)) {
        ft_putstr_fd(BOLD_INTENSE_RED_COLOR, fd);
    } else {
        ft_putstr_fd(BOLD_INTENSE_GREEN_COLOR, fd);
    }
    ft_malloc_print_bytes(fd, data, size);
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
        ft_putstr_fd(BOLD_INTENSE_BLUE_COLOR, fd);
        ft_malloc_print_bytes(fd, page, sizeof(*page));
        t_medium_block* block = get_first_medium_block_(page);
        while (block != NULL) {
            print_medium_block_(fd, block);
            block = get_next_medium_block(block);
        }
    }
}

static void print_large_(int fd, t_large_page* pages) {
    for (t_large_page* page; page != NULL; page = page->next) {
        ft_putstr_fd(BOLD_INTENSE_CYAN_COLOR, fd);
        ft_malloc_print_bytes(fd, page, sizeof(page));
        void* data = get_large_page_data(page);
        size_t size = get_large_page_data_size(page);
        ft_putstr_fd(BOLD_INTENSE_GREEN_COLOR, fd);
        ft_malloc_print_bytes(fd, data, size);
        t_large_page_end* end = get_large_page_end(page);
        ft_putstr_fd(BOLD_INTENSE_CYAN_COLOR, fd);
        ft_malloc_print_bytes(fd, end, sizeof(end));
    }
}

void show_alloc_mem_ex(void) {
    struct s_heap* heap = &g_heap;
    int fd = STDIN_FILENO;

    print_small_(fd, heap->small_pages);
    print_medium_(fd, heap->medium_pages);
    print_large_(fd, heap->large_pages);
}

#else

#include <stdint.h>

#include "ft_malloc.h"
#include "defines.h"
#include "colors.h"
#include "libft.h"
#include "../memory/memory_page/memory_page.h"
#include "../memory/alloc_block/alloc_block.h"

static void print_bytes_(void* ptr, size_t size) {
    uint8_t* bytes = (uint8_t*)ptr;
    for (size_t i = 0; i < size; ++i) {
        ft_printf(" %b ", bytes[i]);
    }
}

static void print_alloc_block_(t_alloc_block* block) {
    const size_t size = get_alloc_size(block);
    ft_printf("block from %p to %p, size %u, is_allocated: %d, is_last_block: %d\n",
        block,
        (void*)((size_t)block + sizeof(*block) + size),
        size,
        is_allocated_block(block),
        is_last_alloc_block(block)
    );
    ft_putstr(BOLD_PURPLE_COLOR);
    print_bytes_(block, sizeof(*block));
    if (is_allocated_block(block)) {
        ft_putstr(BOLD_RED_COLOR);
    } else {
        ft_putstr(BOLD_GREEN_COLOR);
    }
    print_bytes_(get_alloc_data(block), size);
    ft_printf("%s\n", RESET_COLOR);
}

static void print_alloc_blocks_(t_alloc_block* blocks) {
    for (t_alloc_block* block = blocks; block != NULL;) {
        print_alloc_block_(block);
        block = get_next_block_in_memory(block);
    }
}

static void print_memory_page_(t_memory_page* page) {
    ft_printf("page from %p to %p\n", page, (char*)page + page->size);
    ft_putstr(BOLD_YELLOW_COLOR);
    uint8_t* bytes = (uint8_t*)page;
    for (size_t i = 0; i < sizeof(*page); ++i) {
        ft_printf(" %b ", bytes[i]);
    }
    ft_printf("%s\n", RESET_COLOR);
    if (
        page->allocated_list != NULL &&
        (size_t)page->allocated_list < (size_t)page->free_list
    ) {
        print_alloc_blocks_(page->allocated_list);
    } else {
        print_alloc_blocks_(page->free_list);
    }
    ft_printf("%s\n", RESET_COLOR);
}

static void print_memory_pages_(t_memory_page* pages) {
    for (t_memory_page* page = pages; page != NULL; page = page->next) {
        print_memory_page_(page);
    }
}

void show_alloc_mem_ex(void) {
    ft_putstr_color(RED_COLOR, "show_alloc_mem_ex\n");
    // t_memory_page* pages = g_heap.tiny_pages;
    ft_putstr_color(CYAN_COLOR, "tiny pages:\n");
    print_memory_pages_(g_heap.tiny_pages);
    print_memory_pages_(g_heap.small_pages);
    print_memory_pages_(g_heap.large_pages);
}
#endif
