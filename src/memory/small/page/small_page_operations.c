
#include <unistd.h>
#include <sys/mman.h>

#include "small_page.h"
#include "../block/small_block.h"
#include "defines.h"

void* call_mmap(size_t size, int additional_flags);

static void init_small_block_list_(t_small_page* page) {
    t_small_block* list = (t_small_block*)((size_t)page + sizeof(*page));
    list->curr.size = page->size - (sizeof(*list) + sizeof(*page));
    set_block_flag(&list->curr, IS_ALLOCATED_FLAG, false);
    set_block_flag(&list->curr, IS_LAST_BLOCK_FLAG, true);
    list->prev.size = 0;
    page->block_list = list;
}

t_small_page* create_small_page(
    size_t size,
    int additional_mmap_flags,
    t_small_page** page_list
) {
    size = ALIGN_ALLOC_SIZE(size, (size_t)getpagesize());
    t_small_page* page = call_mmap(
        size,
        additional_mmap_flags
    );
    if (page == NULL) {
        return NULL;
    }
    page->next = NULL;
    page->prev = NULL;
    page->size = size;
    init_small_block_list_(page);
    add_to_small_page_list(page_list, page);
    return page;
}

bool destroy_small_page(t_small_page* page, t_small_page* page_list) {
    if (delete_from_small_page_list(page_list, page) == NULL) {
        return false;
    }
    if (munmap((void*)page, page->size) == -1) {
        return false;
    }
    return true;
}
