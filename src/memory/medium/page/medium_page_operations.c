
#include <unistd.h>
#include <sys/mman.h>

#include "medium_page.h"
#include "../block/medium_block.h"
#include "defines.h"

void* call_mmap(size_t size, int additional_flags);

static void init_medium_block_list_(t_medium_page* page) {
    t_medium_block* list = (t_medium_block*)((size_t)page + sizeof(*page));
    list->curr.size = page->size - (sizeof(*list) + sizeof(*page));
    set_block_flag(&list->curr, IS_ALLOCATED_FLAG, false);
    set_block_flag(&list->curr, IS_LAST_BLOCK_FLAG, true);
    list->prev.size = 0;
    set_block_flag(&list->prev, IS_LAST_BLOCK_FLAG, true);
    list->next_ptr = NULL;
    list->prev_ptr = NULL;
    page->block_list = list;
}

t_medium_page* create_medium_page(
    size_t size,
    int additional_mmap_flags,
    t_medium_page** page_list
) {
    size = ALIGN_ALLOC_SIZE(size, (size_t)getpagesize());
    t_medium_page* page = call_mmap(
        size,
        additional_mmap_flags
    );
    if (page == NULL) {
        return NULL;
    }
    page->next = NULL;
    page->prev = NULL;
    page->size = size;
    init_medium_block_list_(page);
    add_to_medium_page_list(page_list, page);
    return page;
}

bool destroy_medium_page(t_medium_page* page, t_medium_page** page_list) {
    if (delete_from_medium_page_list(page_list, page) == NULL) {
        return false;
    }
    if (munmap((void*)page, page->size) == -1) {
        return false;
    }
    return true;
}
