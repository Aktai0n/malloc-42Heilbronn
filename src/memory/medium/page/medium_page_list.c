
#include "medium_page.h"
#include "utils.h"

void add_to_medium_page_list(t_medium_page** list, t_medium_page* new_page) {
    if (*list != NULL) {
        new_page->next = *list;
        (*list)->prev = new_page;
    }
    *list = new_page;
}

t_medium_page* delete_from_medium_page_list(
    t_medium_page** list,
    t_medium_page* to_remove
) {
    if (*list == to_remove) {
        *list = to_remove->next;
    }
    t_medium_page* prev = to_remove->prev;
    t_medium_page* next = to_remove->next;
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

t_medium_page* find_in_medium_page_list(
    const void* ptr,
    t_medium_page* list
) {
    while (list != NULL) {
        if (ft_is_in_region(ptr, list, list->size)) {
            return list;
        }
        list = list->next;
    }
    return NULL;
}
