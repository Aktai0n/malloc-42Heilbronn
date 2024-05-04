
#include <errno.h>

#include "../src/memory/medium/page/medium_page.h"
#include "defines.h"
#include "ft_malloc_internal.h"
#include "libft.h"
#include "colors.h"

#define NUM_PAGES 10

static const char*
test_medium_page_list_(t_medium_page** pages) {
    (void)pages;
    t_medium_page page1 = {
        .block_list = NULL,
        .next = NULL,
        .prev = NULL,
        .size = 100
    };
    t_medium_page page2 = {
        .block_list = NULL,
        .next = NULL,
        .prev = NULL,
        .size = 200
    };
    t_medium_page* list = NULL;

    // test adding elements
    add_to_medium_page_list(&list, &page1);
    if (list != &page1) {
        return "Adding to empty list failed";
    }
    add_to_medium_page_list(&list, &page2);
    if ((list->next != &page1 && list->next != &page2) ||
        list->next->prev != list
    ) {
        return "Adding to not empty list failed";
    }

    // test removing elements
    t_medium_page* page_ptr = delete_from_medium_page_list(&list, &page1);
    if (page_ptr != &page1 || list != &page2 ||
        list->next != NULL || list->prev != NULL
    ) {
        return "Removing from not empty list failed";
    }
    page_ptr = delete_from_medium_page_list(&list, &page2);
    if (page_ptr != &page2 || list != NULL) {
        return "Removing last item from list failed";
    }
    return NULL;
}

static const char*
test_create_medium_page_(t_medium_page** pages) {
    for (size_t i = 1; i < NUM_PAGES; ++i) {
        t_medium_page* page = create_medium_page(SMALL_PAGE_SIZE, 0, pages);
        if (page == NULL) {
            return "Page creation failed";
        }
    }
    return NULL;
}

static const char*
test_find_in_medium_page_list_(t_medium_page** pages) {
    t_medium_page* page_list = *pages;
    t_medium_block* block = page_list->block_list;
    t_medium_page* page = page_list;

    if (find_in_medium_page_list((void*)block, page_list) != page) {
        return "pointer to first page not found";
    }
    for (size_t i = 0; i < NUM_PAGES / 2; ++i) {
        page = page->next;
    }
    block = page->block_list;
    if (find_in_medium_page_list((void*)block, page_list) != page) {
        return "pointer to middle page not found";
    }
    for (; page->next != NULL; page = page->next);
    block = page->block_list;
    if (find_in_medium_page_list((void*)block, page_list) != page) {
        return "pointer to last page not found";
    }

    void* invalid_ptr = &page_list;
    if (find_in_medium_page_list(invalid_ptr, page_list) != NULL) {
        return "invalid pointer found in page list";
    } else if (find_in_medium_page_list(NULL, page_list) != NULL) {
        return "NULL pointer found in page list";
    }
    return NULL;
}

static const char*
test_destroy_medium_page_(t_medium_page** pages) {
    t_medium_page* page = (*pages)->next;
    for (size_t i = 3; i < NUM_PAGES; ++i) {
        t_medium_page* next = page->next;
        t_medium_page* prev = page->prev;
        int save_errno = errno;
        if (destroy_medium_page(page, pages) == false) {
            return "Valid page could not be destroyed";
        }
        if (errno != save_errno) {
            return "Munmap() failed";
        } else if (next != NULL && next->prev == page) {
            return "Next page still has ptr to deleted page";
        } else if (prev != NULL && prev->next == page) {
            return "Prev page still has ptr to deleted page";
        }
        page = next;
    }
    if ((*pages)->next->next != NULL) {
        return "Not all deleted pages erased from page list";
    }
    return NULL;
}

static void
execute_test_(
    const char* (*func)(t_medium_page**),
    const char* test_name,
    t_medium_page** list
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

void test_medium_page(struct s_heap* heap) {
    // t_medium_page** pages = &heap->medium_pages;
    t_medium_page* page = NULL;
    t_medium_page** pages = &page;

    ft_putstr_color(BOLD_INTENSE_YELLOW_COLOR, "Tests for medium pages:\n\n");

    execute_test_(test_medium_page_list_, "medium page list add / remove: ", NULL);

    execute_test_(test_create_medium_page_, "create medium pages: ", pages);

    execute_test_(test_find_in_medium_page_list_, "find in medium page list: ", pages);

    execute_test_(test_destroy_medium_page_, "destroy medium page: ", pages);
}
