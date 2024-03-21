
#include "../src/memory/large/large_page.h"
#include "ft_malloc_internal.h"
#include "defines.h"
#include "libft.h"
#include "colors.h"


const size_t NUM_PAGES = 10;

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
    t_large_page* prev = NULL;
    for (size_t i = 1; i < NUM_PAGES; ++i) {
        t_large_page* page = create_large_page(SMALL_PAGE_SIZE * i, 0, page_list);
        if (page == NULL) {
            return "Page creation failed";
        } else if (prev != page->prev) {
            return "Prev pointer was not set correctly";
        }
        prev = page;
    }
    return NULL;
}

static const char*
test_get_large_page_data_size_(t_large_page** page_list) {
    t_large_page* page = *page_list;
    for (size_t i = 1; i < NUM_PAGES; ++i) {
        const size_t data_size = get_large_page_data_size(page);
        if (data_size < SMALL_PAGE_SIZE * i) {
            return "Data size too small";
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
test_realloc_large_page_(t_large_page** page_list) {
    t_large_page* page = *page_list;
    for (size_t i = 0; page != NULL; ++i) {
        const size_t new_size = i % 2 == 0 ? page->size * 2 : page->size / 2;
        char* first = (char*)get_large_page_data(page);
        char* last = ((char*)get_large_page_end(page)) - 1;
        *first = 'a';
        *last = 'z';
        page = realloc_large_page(page, new_size, page_list);
        first = (char*)get_large_page_data(page);
        last = ((char*)get_large_page_end(page)) - 1;
        if (page->size < new_size) {
            return "Page too small after reallocation";
        } else if (*first != 'a' || *last != 'z') {
            return "Content copying failed";
        }
        page = page->next;
    }

}

static const char*
test_destroy_large_page_(t_large_page** page_list) {
    
}


static void
execute_test_(
    const char* (*test_func(t_large_page**)),
    const char* test_name,
    t_large_page** list
) {
    ft_putstr_color(BOLD_CYAN_COLOR, test_name);
    const char* error_str = test_func(list);
    if (error_str != NULL) {
        ft_putstr_color(BOLD_RED_COLOR, "failed\n");
        ft_printf("Reason: %s\n", error_str);
    } else {
        ft_putstr_color(BOLD_GREEN_COLOR, "passed\n");
    }
    ft_putstr("\n");
}

void test_large_page(struct s_heap heap) {
    t_large_page** pages = &heap.large_pages;

    execute_test_(test_large_page_list_, "large page list add / delete: ", NULL);

    execute_test_(test_create_large_page_, "create large pages: ", pages);

    execute_test_(test_get_large_page_data_size_, "large page data size: ", pages);

    execute_test_(test_get_large_page_end_, "get large page end: ", pages);

    execute_test_(test_large_page_is_corrupted_, "large page is corrupted: ", pages);
}