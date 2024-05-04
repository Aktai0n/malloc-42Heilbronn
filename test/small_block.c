
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
        set_block_flag(&block->prev, IS_LAST_BLOCK_FLAG, i == 0);
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
    t_small_block* block_list = block_arr;
    t_small_block* block = find_small_block(block_list, sizeof(t_small_block));
    if (find_small_block(block_list, 0) == NULL) {
        return "Size 0 not found in block list";
    } else if (block == NULL || get_block_size(block->curr) < sizeof(*block)) {
        return "Block with sizeof t_small_block not found in list";
    } else if (find_small_block(block_list, TINY_PAGE_SIZE) != NULL) {
        return "Too big block found in block list";
    }

    block = get_next_small_block(block_list);
    const size_t size = get_block_size(block->curr);
    char* data = get_small_block_data(block);
    size_t* data_end = (size_t*)(data + size);
    size_t save_data = *data_end;
    *((char*)data_end) = 0;
    if (!small_block_is_corrupted(block)) {
        return "Off-by-one overflow not recognized";
    }
    *data_end = save_data;
    for (size_t i = 0; i < size * 2; ++i) {
        data[i] = (char)i;
    }
    if (!small_block_is_corrupted(block)) {
        return "Complete overflow not recognized";
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
test_merge_small_block_(t_small_block** blocks) {
    t_small_block* block = get_next_small_block(*blocks);
    t_small_block* next = get_next_small_block(block);
    if (block == NULL || next == NULL || is_allocated(next->curr)) {
        return "Not enough blocks to test";
    }
    t_small_block* old_ptr = block;
    t_small_block old_block = *block;
    merge_small_block(&block, false);
    next = get_next_small_block(block);
    if (block != old_ptr) {
        return "Pointer to block was moved when it shouln't have been";
    } else if (next != NULL && !is_allocated(next->curr)) {
        return "Block not merged with next free block";
    } else if (get_block_size(block->curr) <= get_block_size(old_block.curr)) {
        return "Block size not properly updated";
    } else if (is_allocated(old_block.curr) != is_allocated(block->curr)) {
        return "Is allocated flag not transferred correctly";
    }

    old_ptr = block;
    old_block = *block;
    t_small_block* prev = get_prev_small_block(block);
    if (is_allocated(prev->curr)) {
        return "Block layout unsuitable";
    }
    merge_small_block(&block, true);
    prev = get_prev_small_block(block);
    if (block == old_ptr) {
        return "Pointer to block not updated";
    } else if (prev != NULL && !is_allocated(prev->curr)) {
        return "Block not merged mit previous free block";
    } else if (get_block_size(block->curr) <= get_block_size(old_block.curr)) {
        return "Block size not properly updated";
    } else if (is_allocated(old_block.curr) != is_allocated(block->curr)) {
        return "Is allocated flag not transferred correctly";
    }
    return NULL;
}

static const char*
test_allocate_small_block_(t_small_block** blocks) {
    for (size_t i = 0; i < NUM_BLOCKS; ++i) {
        const size_t size = ALIGN_ALLOC_SIZE(
            i % TINY_ALLOC_BLOCK_SIZE,
            FT_MALLOC_ALIGNMENT
        );
        t_small_block* block = allocate_small_block(size, *blocks);
        t_small_block* next = get_next_small_block(block);
        if (block == NULL) {
            return "Block overhead too big";
        } else if (get_block_size(block->curr) < size) {
            return "Block size too small";
        } else if (!is_allocated(block->curr)) {
            return "Block not marked as allocated";
        } else if (next != NULL && !is_allocated(next->prev)) {
            return "Is Allocated flag not set in next block";
        }
    }
    for (t_small_block* block = *blocks; block != NULL;) {
        t_small_block* next = get_next_small_block(block);
        t_small_block* prev = get_prev_small_block(block);
        if (prev != NULL && get_next_small_block(prev) != block) {
            return "Prev block not properly set";
        } else if (next != NULL && (!is_allocated(next->prev) ||
            get_prev_small_block(next) != block)) {
            return "Next block not properly set";
        }
        block = next;
    }
    return NULL;
}

static const char*
test_reallocate_small_block_(t_small_block** blocks) {
    t_small_block* block = *blocks;
    for (size_t i = 0; i < NUM_BLOCKS; ++i) {
        if (block == NULL) {
            return "Not enough blocks to test with";
        }
        t_small_block* next = get_next_small_block(block);
        const size_t new_size = ALIGN_ALLOC_SIZE(
            (i * 2) % TINY_ALLOC_BLOCK_SIZE,
            FT_MALLOC_ALIGNMENT
        );
        char* data = get_small_block_data(block);
        size_t size = get_block_size(block->curr);
        if (size != 0) {
            const size_t pos = i % size;
            const char c = (char)(i % 26) + 'A';
            data[0] = c;
            data[pos] = c;
            data[size - 1] = c;
            block = reallocate_small_block(block, new_size, *blocks);
            data = get_small_block_data(block);
            if (block == NULL) {
                return "Page not suitable for testing";
            }
            if (get_block_size(block->curr) < new_size) {
                return "Size not big enough after realloc";
            } else if (data[0] != c || data[pos] != c || data[size - 1] != c) {
                return "Data not properly copied";
            }
        }
        block = next;
    }
    return NULL;
}

static const char*
test_deallocate_small_block_(t_small_block** blocks) {
    t_small_block* block = *blocks;
    for (size_t i = 0; i < NUM_BLOCKS; ++i) {
        if (block == NULL) {
            return "Not enough blocks to test with";
        }
        deallocate_small_block(&block);
        t_small_block* next = get_next_small_block(block);
        t_small_block* prev = get_prev_small_block(block);
        if (is_allocated(block->curr)) {
            return "Is Allocated flag not reset";
        } else if (next != NULL && !is_allocated(next->curr)) {
            return "Not merged with next free block";
        } else if (prev != NULL && !is_allocated(prev->curr)) {
            return "Not merged with prev free block";
        } else if (next != NULL && is_allocated(next->prev)) {
            return "Is Allocated flag not reset in next block";
        }
        block = next;
    }
    return NULL;
}

static const char*
test_get_small_block_data_(t_small_block** blocks) {
    for (t_small_block* block = *blocks; block != NULL;) {
        void* data = get_small_block_data(block);
        size_t size = get_block_size(block->curr);
        size = (size == 0 ? 1 : size) + sizeof(*block);
        if (!ft_is_in_region(data, block, size)) {
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

void test_small_block(struct s_heap* heap) {
    // t_small_block** blocks = &heap->small_pages->block_list;
    t_small_page* page = NULL;
    page = create_small_page(TINY_PAGE_SIZE, 0, &page);
    if (page == NULL) {
        ft_putstr_color(
            BOLD_INTENSE_RED_COLOR,
            "Tests for small blocks not performed because page creation failed"
        );
        return;
    }
    t_small_block** blocks = &page->block_list;

    ft_putstr_color(BOLD_INTENSE_YELLOW_COLOR, "Tests for small blocks:\n\n");

    execute_test_(test_small_block_list_, "small block list functions: ", NULL);

    execute_test_(test_split_small_block_, "split small block: ", blocks);

    execute_test_(test_merge_small_block_, "merge small block: ", blocks);

    execute_test_(test_allocate_small_block_, "allocate small block: ", blocks);

    execute_test_(test_get_small_block_data_, "get small block data: ", blocks);

    execute_test_(test_reallocate_small_block_, "reallocate small block: ", blocks);

    execute_test_(test_deallocate_small_block_, "deallocate small block: ", blocks);
}
