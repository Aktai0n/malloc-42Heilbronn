
#include <errno.h>

#include "small.h"
#include "block/small_block.h"
#include "page/small_page.h"
#include "defines.h"

t_small_block* allocate_small(
    const size_t size,
    const bool set_zero,
    t_small_page** pages
) {
    for (t_small_page* page = *pages; page != NULL; page = page->next) {
        t_small_block* block = allocate_small_block(size, page->block_list);
        if (block != NULL) {
            if (set_zero) {
                ft_malloc_bzero(
                    get_small_block_data(block),
                    get_small_block_data_end(block)
                );
            }
            return block;
        }
    }
    t_small_page* page = create_small_page(TINY_PAGE_SIZE, 0, pages);
    if (page == NULL) {
        return NULL;
    }
    // No need to zero out the block because new pages are
    // zeroed by the kernel
    return allocate_small_block(size, page->block_list);
}

t_small_block* reallocate_small(
    t_small_block* block,
    const size_t size,
    t_small_page* page,
    t_small_page** pages
) {
    t_small_block* new_block = reallocate_small_block(
        block,
        size,
        page->block_list
    );
    if (new_block != NULL) {
        return new_block;
    }

    new_block = allocate_small(size, false, pages);
    if (new_block != NULL) {
        copy_small_block_data(block, new_block);
        deallocate_small(block, page, pages);
    }
    return new_block;
}

static bool is_last_page_(
    const t_small_page* page,
    const t_small_page* page_list
) {
    return page_list == page && page->next == NULL;
}

static bool is_last_block_on_page_(
    t_small_block* block,
    const t_small_page* page
) {
    return block == page->block_list &&
        is_last_block(block->curr);
}

bool deallocate_small(
    t_small_block* block,
    t_small_page* page,
    t_small_page** page_list
) {
    if (!is_allocated(block->curr)) {
        // double free
        errno = EACCES;
        return false;
    }

    deallocate_small_block(&block);
    if (is_last_block_on_page_(block, page) &&
        !is_last_page_(page, *page_list)
    ) {
        return destroy_small_page(page, page_list);
    }
    return true;
}
