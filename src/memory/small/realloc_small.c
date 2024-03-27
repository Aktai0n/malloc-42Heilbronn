
#include "block/small_block.h"
#include "page/small_page.h"
#include "utils.h"

t_small_block* realloc_small(
    t_small_block* block,
    const size_t size,
    t_small_block* block_list,
    t_small_page* pages
) {
    t_small_block* new_block = reallocate_small_block(
        block,
        size,
        block_list
    );
    if (new_block != NULL) {
        return new_block;
    }

    for (t_small_page* page = pages; page != NULL; page = page->next) {
        if ((size_t)block >= (size_t)page &&
            (size_t)block < (size_t)page + page->size
        ) {
            continue;
        }
        new_block = allocate_small_block(size, page->block_list);
        if (new_block != NULL) {
            copy_small_block_data(block, new_block);
            deallocate_small_block(&block);
            return new_block;
        }
    }
    return NULL;
}