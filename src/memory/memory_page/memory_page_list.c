
#include "memory_page.h"

void add_to_page_list(t_memory_page** list, t_memory_page* new_page) {
    if (*list != NULL) {
        new_page->next = *list;
    }
    *list = new_page;
}

t_memory_page* delete_from_page_list(t_memory_page** list, t_memory_page* to_remove) {
    t_memory_page* prev = NULL;
    for (t_memory_page* page = *list; page != NULL;) {
        if (page == to_remove) {
            if (page == *list) {
                *list = page->next;
            } else {
                prev->next = page->next;
            }
            page->next = NULL;
            return page;
        }
        prev = page;
        page = page->next;
    }
    return NULL;
}

