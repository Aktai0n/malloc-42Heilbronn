
#include <stdbool.h>
#include <errno.h>

#include "memory_page.h"
#include "ft_malloc_internal.h"

bool reclaim_memory_page(t_memory_page* page) {
    t_memory_page** page_list = NULL;
    if (page->type == TINY_PAGE) {
        page_list = &g_heap.small_pages;
    } else if (page->type == SMALL_PAGE) {
        page_list = &g_heap.medium_pages;
    } else {
        page_list = &g_heap.large_pages;
    }

    if (delete_from_page_list(page_list, page) == NULL) {
        errno = EINVAL;
        return false;
    }
    return call_munmap(page);
}
