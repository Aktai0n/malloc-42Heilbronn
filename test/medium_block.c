
#include <errno.h>

#include "../src/memory/medium/block/medium_block.h"
#include "../src/memory/medium/page/medium_page.h"
#include "defines.h"
#include "ft_malloc_internal.h"
#include "libft.h"
#include "colors.h"

#define NUM_BLOCKS 50

static const char*
test_medium_block_list_(t_medium_block** blocks) {



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
    t_medium_block** blocks = &g_heap.small_pages;
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
}
