
#include "ft_malloc_internal.h"
#include "libft.h"
#include "../memory/small/page/small_page.h"
#include "../memory/small/block/small_block.h"

void check_small(t_small_page* page) {
    t_small_block* block = page->block_list;
    while (block != NULL) {
        if (small_block_is_corrupted(block)) {
            t_small_block* next = get_next_small_block(block);
            if (next == NULL) {
                break;
            }
            size_t i = 0;
            for (; i < sizeof(block->curr); ++i) {
                const char* block_size_ptr = ((uintptr_t)&block->curr) + i;
                const char* next_size_ptr = ((uintptr_t)&next->prev) + i;
                if (*block_size_ptr != *next_size_ptr) {
                    break;
                }
            }
            ft_dprintf(
                STDERR_FILENO,
                "block corruption at block %p, size %u\n"
                "corruption of at least %u bytes",
                block,
                get_block_size(block->curr),
                i
            );
        }
        block = get_next_small_block(block);
    }
}


void check_heap(void) {
    struct s_heap* heap = &g_heap;


}
