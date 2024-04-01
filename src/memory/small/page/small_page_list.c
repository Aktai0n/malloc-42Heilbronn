
#include "small_page.h"
#include "utils.h"

void add_to_small_page_list(t_small_page** list, t_small_page* new_page) {
    if (*list != NULL) {
        new_page->next = *list;
        (*list)->prev = new_page;
    }
    *list = new_page;
}

t_small_page* delete_from_small_page_list(
    t_small_page** list,
    t_small_page* to_remove
) {
    if (*list == to_remove) {
        *list = to_remove->next;
    }
    t_small_page* prev = to_remove->prev;
    t_small_page* next = to_remove->next;
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

t_small_page* find_in_small_page_list(
    const void* ptr,
    t_small_page* list
) {
    while (list != NULL) {
        if (ft_is_in_region(ptr, list, list->size)) {
            return list;
        }
        list = list->next;
    }
    return NULL;
}
