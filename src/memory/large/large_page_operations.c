
#include <unistd.h>
#include <sys/mman.h>

#include "large_page.h"
#include "../memory_page/memory_page.h"
#include "defines.h"
#include "utils.h"

static void init_large_page_end_(t_large_page* page) {
    t_large_page_end* end = get_large_page_end(page);
    end->size = page->size;
}

t_large_page* create_large_page(
    size_t size,
    int additional_mmap_flags,
    t_large_page** page_list
) {
    size += sizeof(t_large_page) + sizeof(t_large_page_end);
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
    init_large_page_end_(page);
    add_to_large_page_list(page_list, page);
    return page;
}

bool destroy_large_page(t_large_page* page, t_large_page** page_list) {
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
    const size_t page_size = get_large_page_data_size(page);
    if (page_size >= size) {
        return page;
    }

    t_large_page* new_page = create_large_page(size, 0, page_list);
    if (new_page != NULL) {
        ft_malloc_memcpy(
            get_large_page_data(page),
            get_large_page_data(new_page),
            page_size
        );
    }
    destroy_large_page(page, page_list);
    return new_page;
}

