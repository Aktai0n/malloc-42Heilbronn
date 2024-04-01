
#include "block/small_block.h"
#include "page/small_page.h"
#include "utils.h"
#include "defines.h"

static t_small_block* realloc_small_block_on_other_page_(
    const size_t size,
    t_small_block** old_block,
    t_small_page* page
) {
    t_small_block* new_block =
        allocate_small_block(size, page->block_list);
    if (new_block != NULL) {
        copy_small_block_data(*old_block, new_block);
        deallocate_small_block(old_block);
    }
    return new_block;
}

t_small_block* reallocate_small(
    t_small_block* block,
    const size_t size,
    t_small_block* block_list,
    t_small_page** pages
) {
    t_small_block* new_block = reallocate_small_block(
        block,
        size,
        block_list
    );
    if (new_block != NULL) {
        return new_block;
    }

    for (t_small_page* page = *pages; page != NULL; page = page->next) {
        if (!ft_is_in_region(block, page, page->size)) {
            continue;
        }
        new_block = realloc_small_block_on_other_page_(size, &block, page);
        if (new_block != NULL) {
            return new_block;
        }
    }

    t_small_page* new_page = create_small_page(TINY_PAGE_SIZE, 0, pages);
    if (new_page == NULL) {
        return NULL;
    }
    return realloc_small_block_on_other_page_(size, &block, new_page);
}
