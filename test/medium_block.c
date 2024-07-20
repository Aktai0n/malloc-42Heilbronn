
#include <errno.h>

#include "../src/memory/medium/block/medium_block.h"
#include "../src/memory/medium/page/medium_page.h"
#include "defines.h"
#include "ft_malloc_internal.h"
#include "libft.h"
#include "colors.h"

#define NUM_BLOCKS 50

static const char*
test_medium_block_list_(t_medium_block** unused1, t_medium_block** unused2) {
    unused1 = unused2;
    t_medium_block block_arr[NUM_BLOCKS * 2] = { 0 };
    t_medium_block* alloc_blocks = NULL;
    t_medium_block* free_blocks = NULL;
    t_medium_block* block = NULL;
    t_medium_block* prev = NULL;
    t_medium_block* next = NULL;

    // init block lists
    t_block_base prev_base = { .size = 0 };
    for (size_t i = 0; i < NUM_BLOCKS; ++i) {
        block = block_arr + (i * 2);
        t_block_base* curr = &block->curr;
        if (i % 2 == 0) {
            set_block_flag(curr, IS_ALLOCATED_FLAG, true);
            add_to_medium_block_list(&alloc_blocks, block);
        } else {
            set_block_flag(curr, IS_ALLOCATED_FLAG, false);
            add_to_medium_block_list(&free_blocks, block);
        }
        set_block_size(curr, sizeof(t_medium_block));
        set_block_flag(curr, IS_LAST_BLOCK_FLAG, i + 1 == NUM_BLOCKS);
        block->prev = prev_base;
        set_block_flag(&block->prev, IS_LAST_BLOCK_FLAG, i == 0);
        prev_base = block->curr;
    }

    // memory list tests
    prev = NULL;
    for (size_t i = 0; i < NUM_BLOCKS; ++i) {
        block = block_arr + (i * 2);
        next = NULL;
        if (i + 1 != NUM_BLOCKS) {
            next = block_arr + ((i + 1) * 2);
        }
        if (get_next_medium_block(block) != next) {
            return "Next medium block not found";
        } else if (get_prev_medium_block(block) != prev) {
            return "Prev medium block not found";
        }
        prev = block;
    }

    // linked list tests
    prev = NULL;
    for (block = alloc_blocks; block != NULL; block = block->next_ptr) {
        if (block->prev_ptr != prev) {
            return "prev_ptr not properly set when adding";
        } else if (!is_allocated(block->curr)) {
            return "Free block in allocated list";
        }
        prev = block;
    }
    prev = NULL;
    for (block = free_blocks; block != NULL; block = block->next_ptr) {
        if (block->prev_ptr != prev) {
            return "prev_ptr not properly set when adding";
        } else if (is_allocated(block->curr)) {
            return "Allocated block in free list";
        }
        prev = block;
    }
    block = free_blocks->next_ptr->next_ptr;
    next = block->next_ptr;
    prev = block->prev_ptr;
    block = delete_from_medium_block_list(&free_blocks, block);
    if (next != NULL && next->prev_ptr == block) {
        return "next->prev_ptr not properly reset after deleting";
    } else if (prev != NULL && prev->next_ptr == prev) {
        return "prev->next_ptr not properly reset after deleting";
    }
    block = delete_from_medium_block_list(&alloc_blocks, alloc_blocks);
    if (block == alloc_blocks) {
        return "List head ptr not updated when deleting";
    }



    // corruption tests
    block = get_next_medium_block(block_arr);
    const size_t size = get_block_size(block->curr);
    char* data = get_medium_block_data(block);
    size_t* data_end = get_medium_block_data_end(block);
    size_t save_data = *data_end;
    *((char*)data_end) = 0;
    if (!medium_block_is_corrupted(block)) {
        return "Off-by-one overflow not recognized";
    }
    *data_end = save_data;
    for (size_t i = 0; i < size * 2; ++i) {
        data[i] = (char)i;
    }
    if (!medium_block_is_corrupted(block)) {
        return "Complete overflow not recognized";
    }



    return NULL;
}

static void
execute_test_(
    const char* (*func)(t_medium_block**,t_medium_block**),
    const char* test_name,
    t_medium_block** alloced_list,
    t_medium_block** free_list
) {
    ft_putstr_color(BOLD_CYAN_COLOR, test_name);
    const char* error_str = func(alloced_list, free_list);
    if (error_str != NULL) {
        ft_putstr_color(BOLD_RED_COLOR, "failed\n");
        ft_printf("Reason: %s\n", error_str);
    } else {
        ft_putstr_color(BOLD_GREEN_COLOR, "passed\n");
    }
    ft_putstr("\n");
}

void test_medium_block(struct s_heap* heap) {
    t_medium_page** pages = &g_heap.small_pages;
    t_medium_page* page = NULL;
    page = create_medium_page(SMALL_PAGE_SIZE, 0, &page);
    if (page == NULL) {
        ft_putstr_color(
            BOLD_INTENSE_RED_COLOR,
            "Tests for medium blocks not performed because page creation failed"
        );
        return;
    }
    t_medium_block** alloced_blocks = &page->allocated_list;
    t_medium_block** free_blocks = &page->free_list;

    ft_putstr_color(BOLD_INTENSE_YELLOW_COLOR, "Tests for medium blocks:\n\n");

    execute_test_(test_medium_block_list_, "medium block list functions: ", NULL, NULL);
}
