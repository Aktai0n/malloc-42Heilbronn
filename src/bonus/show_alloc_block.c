
#include "ft_malloc.h"
#include "defines.h"
#include "../memory/memory_page/memory_page.h"
#include "../memory/alloc_block/alloc_block.h"

static const char* bool_to_yes_no_(bool flag) {
    return flag ? "yes" : "no";
}

void show_alloc_block(void* ptr) {
    t_alloc_block* block = get_alloc_block(ptr);
    t_memory_page* page = find_memory_page(block);
    if (page == NULL) {
        return;
    }
    const size_t size = get_alloc_size(block);

    ft_printf("block from %p to %p\n",
        block,
        (void*)((size_t)block + sizeof(*block) + size)
    );
    ft_printf("on page %p to %p\n",
        page,
        (void*)(size_t)page + page->size
    );
    ft_printf("data size: %u, is allocated? %s, is last block on this page? %s\n",
        size,
        bool_to_yes_no_(is_allocated(block)),
        bool_to_yes_no_(is_last_block(block))
    );
}
