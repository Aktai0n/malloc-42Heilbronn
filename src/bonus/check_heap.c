
#include "ft_malloc_internal.h"
#include "libft.h"
#include "../memory/small/page/small_page.h"
#include "../memory/small/block/small_block.h"
#include "../memory/medium/page/medium_page.h"
#include "../memory/medium/block/medium_block.h"
#include "../memory/large/page/large_page.h"

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
                uint8_t* block_size_ptr = (uint8_t*)((uintptr_t)&block->curr) + i;
                uint8_t* next_size_ptr = (uint8_t*)((uintptr_t)&next->prev) + i;
                if (*block_size_ptr == *next_size_ptr) {
                    break;
                }
            }
            ft_dprintf(
                STDERR_FILENO,
                "heap corruption at small block %p, size %u\n"
                "is allocated %d, is last block %d\n"
                "corruption of at least %u bytes\n",
                block,
                get_block_size(block->curr),
                is_allocated(block->curr),
                is_last_block(block->curr),
                i
            );
        }
        block = get_next_small_block(block);
    }
}

t_medium_block* get_first_medium_block_(t_medium_page* page) {
    if (page->allocated_list == NULL && page->free_list == NULL) {
        return NULL;
    }
    t_medium_block* block = page->allocated_list;
    uintptr_t first = UINTPTR_MAX;
    while (block != NULL) {
        if ((uintptr_t)block < first) {
            first = (uintptr_t)block;
        }
        block = block->next_ptr;
    }
    block = page->free_list;
    while (block != NULL) {
        if ((uintptr_t)block < first) {
            first = (uintptr_t)block;
        }
        block = block->next_ptr;
    }
    return (t_medium_block*)first;
}

void check_medium(t_medium_page* page) {
    t_medium_block* block = get_first_medium_block_(page);
    while (block != NULL) {
        if (medium_block_is_corrupted(block)) {
            t_medium_block* next = get_next_medium_block(block);
            if (next == NULL) {
                break;
            }
            size_t i = 0;
            for (; i < sizeof(block->curr); ++i) {
                uint8_t* block_size_ptr = (uint8_t*)((uintptr_t)&block->curr) + i;
                uint8_t* next_size_ptr = (uint8_t*)((uintptr_t)&next->prev) + i;
                if (*block_size_ptr == *next_size_ptr) {
                    break;
                }
            }
            ft_dprintf(
                STDERR_FILENO,
                "heap corruption at medium block %p, size %u\n"
                "is allocated %d, is last block %d\n"
                "corruption of at least %u bytes\n",
                block,
                get_block_size(block->curr),
                is_allocated(block->curr),
                is_last_block(block->curr),
                i
            );
        }
        block = get_next_medium_block(block);
    }
}

void check_large(t_large_page* page) {
    if (large_page_is_corrupted(page)) {
        t_large_page_end* end = get_large_page_end(page);
        size_t i = 0;
        for (; i < sizeof(*end); ++i) {
            uint8_t* page_size_ptr = (uint8_t*)((uintptr_t)&page->size) + i;
            uint8_t* end_size_ptr = (uint8_t*)((uintptr_t)&end->size) + i;
            if (*page_size_ptr == *end_size_ptr) {
                break;
            }
        }
        ft_dprintf(
            STDERR_FILENO,
            "heap corruption at large block %p, size %u\n"
            "corruption of at least %u bytes\n",
            page,
            page->size,
            i
        );
    }
}


void check_heap(void) {
    struct s_heap* heap = &g_heap;

    for (t_small_page* page = heap->small_pages; page != NULL; page = page->next) {
        check_small(page);
    }
    for (t_medium_page* page = heap->medium_pages; page != NULL; page = page->next) {
        check_medium(page);
    }
    for (t_large_page* page = heap->large_pages; page != NULL; page = page->next) {
        check_large(page);
    }
}
