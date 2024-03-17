
#include "../src/memory/large/large_page.h"
#include "ft_malloc_internal.h"
#include "defines.h"
#include "libft.h"
#include "colors.h"


const size_t NUM_PAGES = 10;

bool test_create_large_page(t_large_page** page_list) {
    t_large_page* prev = NULL;
    for (size_t i = 1; i < NUM_PAGES; ++i) {
        t_large_page* page = create_large_page(SMALL_PAGE_SIZE * i, 0, page_list);
        if (page == NULL) {
            return false;
        }
        if (prev != page->prev) {
            return false;
        }
        prev = page;
    }
    return true;
}

bool test_get_large_page_data_size(t_large_page** page_list) {
    t_large_page* page = *page_list;
    for (size_t i = 1; i < NUM_PAGES; ++i) {
        const size_t data_size = get_large_page_data_size(page);
        if (data_size != SMALL_PAGE_SIZE * i) {
            return false;
        }
        page = page->next;
    }
    return true;
}

bool test_get_large_page_end(t_large_page** page_list) {
    for (t_large_page* page = *page_list; page != NULL; page = page->next) {
        t_large_page_end* end = get_large_page_end(page);
        if ((size_t)end > (size_t)page + page->size ||
            (size_t)end < (size_t)page
        ) {
            return false;
        }
    }
    return true;
}

bool test_large_page_is_corrupted(t_large_page** page_list) {
    for (t_large_page* page = *page_list; page != NULL; page = page->next) {

    }
}

bool test_destroy_large_page(t_large_page** page_list) {
    
}


void execute_test(
    bool (*test_func(t_large_page**)),
    const char* test_name,
    t_large_page** list
) {
    ft_putstr_color(BOLD_CYAN_COLOR, test_name);
    if (!test_func(list)) {
        ft_putstr_color(BOLD_RED_COLOR, "failed\n");
    } else {
        ft_putstr_color(BOLD_GREEN_COLOR, "passed\n");
    }
    ft_putstr("\n");
}

void test_large_page(struct s_heap heap) {
    t_large_page** pages = &heap.large_pages;

    execute_test(test_create_large_page, "create large pages: ", pages);

    execute_test(test_get_large_page_data_size, "large page data size: ", pages);

    execute_test(test_get_large_page_end, "get large page end: ", pages);

    
}