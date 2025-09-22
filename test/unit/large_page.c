
#include <errno.h>

#include "../src/memory/large/page/large_page.h"
#include "ft_malloc_internal.h"
#include "defines.h"
#include "libft.h"
#include "colors.h"

#define NUM_PAGES 10

static const char*
test_large_page_list_(t_large_page** page_list) {
    (void)page_list;
    t_large_page page1 = {
        .size = 100,
        .next = NULL,
        .prev = NULL
    };
    t_large_page page2 = {
        .size = 200,
        .next = NULL,
        .prev = NULL
    };
    t_large_page* list = NULL;

    // test adding elements
    add_to_large_page_list(&list, &page1);
    if (list != &page1) {
        return "Adding to empty list failed";
    }
    add_to_large_page_list(&list, &page2);
    if ((list->next != &page1 && list->next != &page2) ||
        list->next->prev != list
    ) {
        return "Adding to not empty list failed";
    }
    
    // test removing elements
    t_large_page* page_ptr = delete_from_large_page_list(&list, &page1);
    if (page_ptr != &page1 || list != &page2 ||
        list->next != NULL || list->prev != NULL
    ) {
        return "Removing from not empty list failed";
    }
    page_ptr = delete_from_large_page_list(&list, &page2);
    if (page_ptr != &page2 || list != NULL) {
        return "Removing last item from list failed";
    }
    return NULL;
}

static const char*
test_create_large_page_(t_large_page** page_list) {
    for (size_t i = 1; i < NUM_PAGES; ++i) {
        t_large_page* page = create_large_page(SMALL_PAGE_SIZE * i, 0, page_list);
        if (page == NULL) {
            return "Page creation failed";
        }
    }
    return NULL;
}

static const char*
test_get_large_page_data_size_(t_large_page** page_list) {
    t_large_page* page = *page_list;
    for (size_t i = 1; i < NUM_PAGES; ++i) {
        const size_t data_size = get_large_page_data_size(page);
        if (data_size > page->size) {
            return "Data size too big";
        }
        page = page->next;
    }
    return NULL;
}

static const char*
test_get_large_page_end_(t_large_page** page_list) {
    for (t_large_page* page = *page_list; page != NULL; page = page->next) {
        t_large_page_end* end = get_large_page_end(page);
        if ((size_t)end > (size_t)page + page->size ||
            (size_t)end < (size_t)page
        ) {
            return "Page end not in page";
        }
    }
    return NULL;
}

static const char*
test_large_page_is_corrupted_(t_large_page** page_list) {
    for (t_large_page* page = *page_list; page != NULL; page = page->next) {
        if (large_page_is_corrupted(page)) {
            return "False positive";
        }
    }

    t_large_page* page = NULL;
    create_large_page(100, 0, &page);
    char* ptr = (char*)page;
    for (size_t i = sizeof(*page); i < page->size; ++i) {
        ptr[i] = 'A';
    }
    if (!large_page_is_corrupted(page)) {
        return "False negative";
    }
    return NULL;
}

static const char*
test_find_in_large_page_list_(t_large_page** page_list) {
    t_large_page* find_valid = (*page_list)->next;
    const void* valid_data = get_large_page_data(find_valid);
    t_large_page_end* valid_end = get_large_page_end(find_valid);
    const void* invalid_end = (void*)((size_t)find_valid + find_valid->size);

    if (find_in_large_page_list((void*)find_valid, *page_list) != find_valid) {
        return "page pointer not found";
    } else if (find_in_large_page_list(valid_data, *page_list) != find_valid) {
        return "data pointer not found";
    } else if (find_in_large_page_list((void*)valid_end, *page_list) != find_valid) {
        return "end pointer not found";
    } else if (find_in_large_page_list(NULL, *page_list) != NULL) {
        return "NULL found in page list";
    } else if (find_in_large_page_list(invalid_end, *page_list) == find_valid) {
        return "off by one error at the end of the page";
    }
    return NULL;
}

static const char*
test_realloc_large_page_(t_large_page** page_list) {
    t_large_page* page = *page_list;
    for (size_t i = 0; page != NULL; ++i) {
        t_large_page* next = page->next;
        const size_t new_size = i % 2 == 0 ? page->size * 2 : page->size / 2;
        char* first = (char*)get_large_page_data(page);
        *first = 'a';
        page = realloc_large_page(page, new_size, page_list);
        first = (char*)get_large_page_data(page);
        if (page->size < new_size) {
            return "Page too small after reallocation";
        } else if (*first != 'a') {
            return "Content copying failed";
        }
        page = next;
    }
    return NULL;
}

static const char*
test_destroy_large_page_(t_large_page** page_list) {
    t_large_page* page = (*page_list)->next;
    for (size_t i = 3; i < NUM_PAGES; ++i) {
        t_large_page* next = page->next;
        t_large_page* prev = page->prev;
        int save_errno = errno;
        if (destroy_large_page(page, page_list) == false) {
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
    if ((*page_list)->next->next != NULL) {
        return "Not all deleted pages erased from page list";
    }
    return NULL;
}

static void
execute_test_(
    const char* (*func)(t_large_page**),
    const char* test_name,
    t_large_page** list
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

void test_large_page(struct s_heap* heap) {
    t_large_page** pages = &heap->large_pages;
    // t_large_page* page = NULL;
    // t_large_page** pages = &page;

    ft_putstr_color(BOLD_INTENSE_YELLOW_COLOR, "Tests for large pages:\n\n");

    execute_test_(test_large_page_list_, "large page list add / remove: ", NULL);

    execute_test_(test_create_large_page_, "create large pages: ", pages);

    execute_test_(test_get_large_page_data_size_, "large page data size: ", pages);

    execute_test_(test_get_large_page_end_, "get large page end: ", pages);

    execute_test_(test_find_in_large_page_list_, "find in large page list: ", pages);

    execute_test_(test_large_page_is_corrupted_, "large page is corrupted: ", pages);
    
    execute_test_(test_realloc_large_page_, "realloc large page: ", pages);

    execute_test_(test_destroy_large_page_, "destroy large page: ", pages);
}
