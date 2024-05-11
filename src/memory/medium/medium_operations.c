
#include <errno.h>

#include "medium.h"
#include "block/medium_block.h"
#include "page/medium_page.h"
#include "defines.h"

t_medium_block* allocate_medium(const size_t size, t_medium_page** pages) {
    for (t_medium_page* page = *pages; page != NULL; page = page->next) {
        t_medium_block* block = allocate_medium_block(
            size,
            &page->free_list,
            &page->allocated_list
        );
        if (block != NULL) {
            return block;
        }
    }
    t_medium_page* page = create_medium_page(SMALL_PAGE_SIZE, 0, pages);
    if (page == NULL) {
        return NULL;
    }
    return allocate_medium_block(size, &page->free_list, &page->allocated_list);
}

t_medium_block* reallocate_medium(
    t_medium_block* block,
    const size_t size,
    t_medium_page* page,
    t_medium_page** pages
) {
    t_medium_block* new_block = reallocate_medium_block(
        block,
        size,
        &page->free_list,
        &page->allocated_list
    );
    if (new_block != NULL) {
        return new_block;
    }

    new_block = allocate_medium(size, pages);
    if (new_block != NULL) {
        copy_medium_block_data(block, new_block);
        deallocate_medium(block, page, pages);
    }
    return new_block;
}

static bool is_last_page_(
    const t_medium_page* page,
    const t_medium_page* pages
) {
    return page == pages && pages->next == NULL;
}

static bool is_last_block_on_page_(
    const t_medium_block* block,
    const t_medium_page* page
) {
    return page->allocated_list != NULL &&
        block == page->allocated_list &&
        block->next_ptr == NULL &&
        block->prev_ptr == NULL;
}

bool deallocate_medium(
    t_medium_block* block,
    t_medium_page* page,
    t_medium_page** pages
) {
    if (!is_allocated(block->curr)) {
        errno = EACCES;
        return false;
    }
    if (is_last_block_on_page_(block, page) &&
        !is_last_page_(page, *pages)
    ) {
        return destroy_medium_page(page, pages);
    }
    return deallocate_medium_block(
        block,
        &page->free_list,
        &page->allocated_list
    );
}