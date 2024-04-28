
#include <errno.h>

#include "../src/memory/small/block/small_block.h"
#include "../src/memory/small/page/small_page.h"
#include "defines.h"
#include "ft_malloc_internal.h"
#include "libft.h"
#include "colors.h"

#define NUM_BLOCKS 50

static const char*
test_small_block_list_(t_small_block** blocks) {
    (void)blocks;
    t_small_block block_arr[NUM_BLOCKS * 2];
    t_block_base prev_base = { .size = 0 };
    for (size_t i = 0; i < NUM_BLOCKS; ++i) {
        t_small_block* block = block_arr + (i * 2);
        t_block_base* curr = &block->curr;
        set_block_size(curr, sizeof(t_small_block));
        set_block_flag(curr, IS_ALLOCATED_FLAG, i % 2 == 0);
        set_block_flag(curr, IS_LAST_BLOCK_FLAG, i + 1 == NUM_BLOCKS);
        block->prev = prev_base;
        prev_base = block->curr;
    }
    
    t_small_block* prev = NULL;
    for (size_t i = 0; i < NUM_BLOCKS; ++i) {
        t_small_block* block = block_arr + (i * 2);
        t_small_block* next = NULL;
        if (i + 1 != NUM_BLOCKS) {
            next = block_arr + ((i + 1) * 2);
        }
        if (get_next_small_block(block) != next) {
            return "next small block not found";
        } else if (get_prev_small_block(block) != prev) {
            return "prev small block not found";
        }
        prev = block;
    }
    return NULL;
}

static const char*
test_get_small_block_data_(t_small_block** blocks) {
    for (t_small_block* block = *blocks; block != NULL;) {
        void* data = get_small_block_data(block);
        if ()
        block = get_next_small_block(block);
    }


    return NULL;
}

static void
execute_test_(
    const char* (*func)(t_small_block**),
    const char* test_name,
    t_small_block** list
) {
    ft_putstr_color(BOLD_CYAN_COLOR, test_name);
    const char* error_str = func(list);
    if (error_str != NULL) {
        ft_putstr_color(BOLD_RED_COLOR, "failed\n");
        ft_printf("Reason: %s\n", error_str);
    } else {
        ft_putstr_color(BOLD_GREEN_COLOR, "passed\n");
    }
    ft_putstr("\n");
}

void test_small_block(t_small_page* pages) {
    // t_small_block** blocks = &pages->block_list;
    t_small_page* page = create_small_page(TINY_PAGE_SIZE, 0, &pages);
    if (page == NULL) {
        ft_putstr_color(
            BOLD_INTENSE_RED_COLOR,
            "Tests for small blocks not performed because page creation failed"
        );
        return;
    }
    t_small_block** blocks = &page->block_list;

    ft_putstr_color(BOLD_INTENSE_YELLOW_COLOR, "Tests for small blocks:\n\n");


}