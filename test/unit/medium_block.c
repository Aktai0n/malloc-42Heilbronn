
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

static t_medium_block*
find_medium_block_in_list_(t_medium_block* list, t_medium_block* to_find) {
    for (;list != NULL; list = list->next_ptr) {
        void* end = get_medium_block_data_end(list);
        if (to_find >= list && (uintptr_t)to_find < (uintptr_t)end) {
            return list;
        }
    }
    return NULL;
}

static size_t
get_medium_block_list_size_(t_medium_block* list) {
    size_t size = 0;
    while (list != NULL) {
        ++size;
        list = list->next_ptr;
    }
    return size;
}

static const char*
test_split_medium_block_(t_medium_block** alloc_blocks, t_medium_block** free_blocks) {
    (void)alloc_blocks;
    t_medium_block* block = *free_blocks;
    size_t free_list_size = get_medium_block_list_size_(block);
    for (size_t i = 0; i < NUM_BLOCKS; ++i) {
        t_medium_block old = *block;
        const size_t size = ALIGN_ALLOC_SIZE(
            i % SMALL_ALLOC_BLOCK_SIZE,
            SMALL_ALLOC_BLOCK_SIZE
        );
        if (!split_medium_block(block, size, free_blocks)) {
            return "Block clould not be splitted";
        }
        size_t new_size = get_medium_block_list_size_(*free_blocks);

        if (get_block_size(block->curr) >= get_block_size(old.curr)) {
            return "Block size not updated";
        } else if (is_allocated(old.curr) != is_allocated(block->curr)) {
            return "Allocation flag not transferred";
        } else if (is_last_block(old.curr) && is_last_block(block->curr)) {
            return "Last block flag not reset";
        } else if (new_size <= free_list_size) {
            return "New block not added to free list";
        }
        t_medium_block* next = get_next_medium_block(block);
        if (next == NULL) {
            return "Next block not properly set";
        } else if (get_prev_medium_block(next) != block) {
            return "Next->prev block not properly set";
        } else if (get_block_size(next->curr) == get_block_size(old.curr)) {
            return "Next block size not properly set";
        } else if (is_allocated(next->curr)) {
            return "Next block marked as allocated";
        } else if (is_last_block(old.curr) != is_last_block(next->curr)) {
            return "Last block flag in Next block not properly set";
        }
        block = next;
        free_list_size = new_size;
    }
    return NULL;
}

static const char*
test_merge_medium_block_(t_medium_block** blocks_start, t_medium_block** free_blocks) {
    t_medium_block* block = get_next_medium_block(*blocks_start);
    t_medium_block* next = get_next_medium_block(block);
    t_medium_block* prev = get_prev_medium_block(block);
    if (next == NULL || prev == NULL || is_allocated(prev->curr) || is_allocated(next->curr)) {
        return "Block layout unsuitable for testing";
    }

    t_medium_block* old_ptr = block;
    t_medium_block old_block = *block;
    if (!merge_medium_block(&block, false, free_blocks)) {
        return "No merge performed even though next was free";
    }
    next = get_next_medium_block(block);
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
    if (!merge_medium_block(&block, true, free_blocks)) {
        return "No merge performed even though prev was free";
    }
    prev = get_prev_medium_block(block);
    if (block == old_ptr) {
        return "Pointer to block not updated";
    } else if (prev != NULL && !is_allocated(prev->curr)) {
        return "Block not merged with previous free block";
    } else if (get_block_size(block->curr) <= get_block_size(old_block.curr)) {
        return "Block size not properly updated";
    } else if (is_allocated(old_block.curr) != is_allocated(block->curr)) {
        return "Is allocated flag not transferred correctly";
    }
    *blocks_start = block;
    *free_blocks = block;
    return NULL;
}

static const char*
test_allocate_medium_block_(t_medium_block** alloc_blocks, t_medium_block** free_blocks) {
    for (size_t i = 0; i < NUM_BLOCKS; ++i) {
        const size_t size = ALIGN_ALLOC_SIZE(
            TINY_ALLOC_BLOCK_SIZE + (i % SMALL_ALLOC_BLOCK_SIZE),
            FT_MALLOC_ALIGNMENT
        );
        t_medium_block* block = allocate_medium_block(size, free_blocks, alloc_blocks);
        t_medium_block* next = get_next_medium_block(block);
        if (block == NULL) {
            return "Block overhead too big";
        } else if (get_block_size(block->curr) < size) {
            return "Block size too small";
        } else if (!is_allocated(block->curr)) {
            return "Block not marked as allocated";
        } else if (next != NULL && !is_allocated(next->prev)) {
            return "Is Allocated flag not set in next block";
        } else if (find_medium_block_in_list_(*alloc_blocks, block) == NULL) {
            return "New block not added to allocated list";
        } else if (find_medium_block_in_list_(*free_blocks, block) != NULL) {
            return "New block not removed from free list";
        }
    }

    for (t_medium_block* block = *alloc_blocks; block != NULL; block = block->next_ptr) {
        t_medium_block* next = get_next_medium_block(block);
        t_medium_block* prev = get_prev_medium_block(block);
        if (prev != NULL && get_next_medium_block(prev) != block) {
            return "Prev block not properly set";
        } else if (next != NULL && (!is_allocated(next->prev) ||
            get_prev_medium_block(next) != block)) {
            return "Next block not properly set";
        }
        next = block->next_ptr;
        prev = block->prev_ptr;
        if (next != NULL && next->prev_ptr != block) {
            return "Next ptr not properly set";
        } else if (prev != NULL && prev->next_ptr != block) {
            return "Prev ptr not properly set";
        }
    }
    for (t_medium_block* block = *free_blocks; block != NULL; block = block->next_ptr) {
        t_medium_block* next = get_next_medium_block(block);
        t_medium_block* prev = get_prev_medium_block(block);
        if (prev != NULL && get_next_medium_block(prev) != block) {
            return "Prev block not properly set";
        } else if (next != NULL && (!is_allocated(next->prev) ||
            get_prev_medium_block(next) != block)) {
            return "Next block not properly set";
        }
        next = block->next_ptr;
        prev = block->prev_ptr;
        if (next != NULL && next->prev_ptr != block) {
            return "Next ptr not properly set";
        } else if (prev != NULL && prev->next_ptr != block) {
            return "Prev ptr not properly set";
        }
    }
    return NULL;
}

static const char*
test_reallocate_medium_block_(t_medium_block** alloc_blocks, t_medium_block** free_blocks) {
    t_medium_block* block = *alloc_blocks;
    for (size_t i = 0; i < NUM_BLOCKS; ++i) {
        if (block == NULL) {
            return "Not enough blocks to test with";
        }
        t_medium_block* next = block->next_ptr;
        const size_t new_size = ALIGN_ALLOC_SIZE(
            TINY_ALLOC_BLOCK_SIZE + ((i * 2) % SMALL_ALLOC_BLOCK_SIZE),
            FT_MALLOC_ALIGNMENT
        );
        char* data = get_medium_block_data(block);
        size_t size = get_block_size(block->curr);
        if (size != 0) {
            const size_t pos = i % size;
            const char c = (char)(i % 26) + 'A';
            data[0] = c;
            data[pos] = c;
            data[size - 1] = c;
            block = reallocate_medium_block(block, new_size, free_blocks, alloc_blocks);
            data = get_medium_block_data(block);
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
test_deallocate_medium_block_(t_medium_block** alloc_blocks, t_medium_block** free_blocks) {
    t_medium_block* block = *alloc_blocks;
    for (size_t i = 0; i < NUM_BLOCKS; ++i) {
        if (block == NULL) {
            return "Not enough blocks to test with";
        }
        t_medium_block* next_ptr = block->next_ptr;
        t_medium_block* prev_ptr = block->prev_ptr;
        deallocate_medium_block(&block, free_blocks, alloc_blocks);
        t_medium_block* next = get_next_medium_block(block);
        t_medium_block* prev = get_prev_medium_block(block);
        if (is_allocated(block->curr)) {
            return "Is Allocated flag not reset";
        } else if (next != NULL && !is_allocated(next->curr)) {
            return "Not merged with next free block";
        } else if (prev != NULL && !is_allocated(prev->curr)) {
            return "Not merged with prev free block";
        } else if (next != NULL && is_allocated(next->prev)) {
            return "Is Allocated flag not reset in next block";
        } else if (find_medium_block_in_list_(*alloc_blocks, block) != NULL) {
            return "Block not removed from alloc list";
        } else if (find_medium_block_in_list_(*free_blocks, block) == NULL) {
            return "Block not added to free list";
        } else if (next_ptr != NULL && block->next_ptr == next_ptr) {
            return "Next ptr not updated";
        } else if (prev_ptr != NULL && block->prev_ptr == prev_ptr) {
            return "Prev ptr not updated";
        }
        block = next_ptr;
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
    (void)heap;
    t_medium_page* page = NULL;
    page = create_medium_page(SMALL_PAGE_SIZE, 0, &page);
    if (page == NULL) {
        ft_putstr_color(
            BOLD_INTENSE_RED_COLOR,
            "Tests for medium blocks not performed because page creation failed"
        );
        return;
    }
    t_medium_block** alloc_blocks = &page->allocated_list;
    t_medium_block** free_blocks = &page->free_list;
    t_medium_block* blocks_start = page->free_list;

    ft_putstr_color(BOLD_INTENSE_YELLOW_COLOR, "Tests for medium blocks:\n\n");

    execute_test_(test_medium_block_list_, "medium block list functions: ", NULL, NULL);

    execute_test_(test_split_medium_block_, "split medium block: ", alloc_blocks, free_blocks);

    execute_test_(test_merge_medium_block_, "merge medium block: ", &blocks_start, free_blocks);

    execute_test_(test_allocate_medium_block_, "allocate medium block: ", alloc_blocks, free_blocks);

    execute_test_(test_reallocate_medium_block_, "reallocate medium block: ", alloc_blocks, free_blocks);

    execute_test_(test_deallocate_medium_block_, "deallocate medium block: ", alloc_blocks, free_blocks);
}
