
#include <unistd.h>

#include "large_page.h"
#include "../memory_page/memory_page.h"
#include "defines.h"

void ft_malloc_memcpy(void* src, void* dst, size_t size);

t_large_page* create_large_page(
    size_t size,
    int additional_mmap_flags,
    t_large_page** page_list
) {
    size = ALIGN_ALLOC_SIZE(size, (size_t)getpagesize());
    t_large_page* page = call_mmap(
        size,
        additional_mmap_flags
    );
    if (page == NULL) {
        return NULL;
    }
    page->next = NULL;
    page->prev = NULL;
    page->size = size;
    add_to_large_page_list(page_list, page);
    return page;
}

bool destroy_large_page(t_large_page** page_list, t_large_page* page) {
    if (delete_from_large_page_list(page_list, page) == NULL) {
        return false;
    }
    if (munmap((void*)page, page->size) == -1) {
        return false;
    }
    return true;
}

t_large_page* realloc_large_page(
    t_large_page* page,
    const size_t size,
    t_large_page** page_list
) {
    if (page->size >= size) {
        return page;
    }

    t_large_page* new_page = create_large_page(size, 0, page_list);
    if (new_page == NULL) {
        destroy_large_page(page_list, page);
        return NULL;
    }
    ft_malloc_memcpy(page, new_page, page->size);
    destroy_large_page(page_list, page);
    return new_page;
}