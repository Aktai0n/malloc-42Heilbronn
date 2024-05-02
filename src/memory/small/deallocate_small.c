
#include <errno.h>

#include "block/small_block.h"
#include "page/small_page.h"
#include "utils.h"
#include "defines.h"

static bool is_last_page_(const t_small_page* page) {
    return page->next == NULL;
}

static bool is_last_block_on_page_(
    const t_small_block* block,
    const t_small_page* page
) {
    return page->block_list == block &&
        is_last_block(block->curr);
}

bool deallocate_small(void* ptr, t_small_page** page_list) {
    t_small_block* block = get_small_block(ptr);
    t_small_page* page = find_in_small_page_list(ptr, *page_list);
    if (page == NULL) {
        return false;
    } else if (!is_allocated(block->curr)) {
        // double free
        errno = EACCES;
        return false;
    }

    if (is_last_block_on_page_(block, page) && !is_last_page_(page)) {
        return destroy_small_page(page, page_list);
    }
    deallocate_small_block(&block);
    return true;
}
