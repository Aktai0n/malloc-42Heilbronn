
#include "ft_malloc.h"
#include "defines.h"
#include "memory_page.h"


enum e_memory_page find_page_type(size_t size) {
    if (size <= TINY_ALLOC_BLOCK_SIZE) {
        return TINY_PAGE;
    } else if (size <= SMALL_ALLOC_BLOCK_SIZE) {
        return SMALL_PAGE;
    } else {
        return LARGE_PAGE;
    }
}

static void add_to_page_list_(t_memory_page** list, t_memory_page* new_page) {
    if (*list != NULL) {
        new_page->next = *list;
    }
    *list = new_page;
}

void* allocate_memory(size_t requested_block_size) {
    requested_block_size = ALIGN_ALLOC_SIZE(requested_block_size);
    // TODO: Look whether there are memory pages with space available
    t_memory_page* page = NULL;
    if (requested_block_size <= TINY_ALLOC_BLOCK_SIZE) {
        page = init_memory_page(TINY_PAGE_SIZE, TINY_PAGE, 0);
        add_to_page_list_(&g_heap.tiny_pages, page);
    } else if (requested_block_size <= SMALL_ALLOC_BLOCK_SIZE) {
        page = init_memory_page(SMALL_PAGE_SIZE, SMALL_PAGE, 0);
        add_to_page_list_(&g_heap.small_pages, page);
    } else {
        page = init_memory_page(requested_block_size, LARGE_PAGE, 0);
        add_to_page_list_(&g_heap.large_pages, page);
    }
    return page;
}