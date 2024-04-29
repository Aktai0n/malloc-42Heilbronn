
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
test_split_small_block_(t_small_block** blocks) {
    t_small_block* block = *blocks;
    for (size_t i = 0; i < NUM_BLOCKS; ++i) {
        t_small_block old = *block;
        const size_t size = ALIGN_ALLOC_SIZE(
            i % TINY_ALLOC_BLOCK_SIZE,
            FT_MALLOC_ALIGNMENT
        );
        if (!split_small_block(block, size)) {
            return "Block clould not be splitted";
        }

        if (get_block_size(block->curr) == get_block_size(old.curr)) {
            return "Block size not updated";
        } else if (is_allocated(old.curr) != is_allocated(block->curr)) {
            return "Allocation flag not transferred";
        } else if (is_last_block(old.curr) && is_last_block(block->curr)) {
            return "Last block flag not reset";
        }
        t_small_block* next = get_next_small_block(block);
        if (next == NULL) {
            return "Next block not properly set";
        } else if (get_prev_small_block(next) != block) {
            return "Next->prev block not properly set";
        } else if (get_block_size(next->curr) == get_block_size(old.curr)) {
            return "Next block size not properly set";
        } else if (is_allocated(next->curr)) {
            return "Next block marked as allocated";
        } else if (is_last_block(old.curr) != is_last_block(next->curr)) {
            return "Last block flag in Next block not properly set";
        }
        block = next;
    }
    return NULL;
}

static const char*
test_merge_small_block();

static const char*
test_allocate_small_block_(t_small_block** blocks) {
    for (size_t i = 0; i < NUM_BLOCKS; ++i) {
        const size_t size = i % TINY_ALLOC_BLOCK_SIZE;
        t_small_block* block = allocate_small_block(size, *blocks);
        if (block == NULL) {
            return "Block overhead too big";
        } else if (get_block_size(block->curr) < size) {
            return "Block size too small";
        } else if (!is_allocated(block->curr)) {
            return "Block not marked as allocated";
        }
    }
    for (t_small_block* block = *blocks; block != NULL;) {
        t_small_block* next = get_next_small_block(block);
        t_small_block* prev = get_prev_small_block(block);
        if (prev != NULL && get_next_small_block(prev) != block) {
            return "Prev block not properly set";
        } else if (next != NULL && (!is_allocated(next->prev) ||
            !get_prev_small_block(next) != block)) {
            return "Next block not properly set";
        }
    }
    return NULL;
}

static const char*
test_get_small_block_data_(t_small_block** blocks) {
    for (t_small_block* block = *blocks; block != NULL;) {
        void* data = get_small_block_data(block);
        if (!ft_is_in_region(data, block, get_block_size(block->curr))) {
            return "block data pointer incorrect";
        } else if (get_small_block(data) != block) {
            return "block pointer invalid";
        }
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