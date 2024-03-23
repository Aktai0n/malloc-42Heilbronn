
#include "large_page.h"



void add_to_large_page_list(t_large_page** list, t_large_page* new_page) {
    if (*list != NULL) {
        new_page->next = *list;
        (*list)->prev = new_page;
    }
    *list = new_page;
}

t_large_page* delete_from_large_page_list(
    t_large_page** list,
    t_large_page* to_remove
) {
    if (*list == to_remove) {
        *list = to_remove->next;
    }
    t_large_page* prev = to_remove->prev;
    t_large_page* next = to_remove->next;
    if (prev != NULL) {
        prev->next = next;
    }
    if (next != NULL) {
        next->prev = prev;
    }
    to_remove->next = NULL;
    to_remove->prev = NULL;
    return to_remove;
}

t_large_page* find_in_large_page_list(
    const void* ptr,
    t_large_page* list
) {
    const size_t ptr_addr = (size_t)ptr;
    while (list != NULL) {
        const size_t page_addr = (size_t)list;
        if (page_addr <= ptr_addr &&
            page_addr + list->size > ptr_addr
        ) {
            return list;
        }
        list = list->next;
    }
    return NULL;
}
