
#include <stdint.h>

#include "ft_malloc.h"
#include "defines.h"
#include "colors.h"
#include "../libft/libft.h"
#include "../memory/memory_page/memory_page.h"
#include "../memory/alloc_block/alloc_block.h"

static void print_bytes_(void* ptr, size_t size) {
    uint8_t* bytes = (uint8_t*)ptr;
    for (size_t i = 0; i < size; ++i) {
        ft_printf(" %b ", bytes[i]);
    }
}

static void print_alloc_block_(t_alloc_block* block) {
    ft_putstr(BOLD_PURPLE_COLOR);
    print_bytes_(block, sizeof(*block));
    if (is_allocated(block)) {
        ft_putstr(BOLD_RED_COLOR);
    } else {
        ft_putstr(BOLD_GREEN_COLOR);
    }
    print_bytes_(block + 1, block->size);
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
    if (
        page->allocated_list != NULL &&
        (size_t)page->allocated_list < (size_t)page->free_list
    ) {
        print_alloc_blocks_(page->allocated_list);
    } else {
        print_alloc_blocks_(page->free_list);
    }
    ft_putstr(RESET_COLOR);

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
}