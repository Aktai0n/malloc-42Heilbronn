
#include "ft_malloc.h"
#include "defines.h"
#include "../memory/memory_page/memory_page.h"
#include "../memory/alloc_block/alloc_block.h"


void show_alloc_block(void* ptr) {
    t_alloc_block* block = get_alloc_block(ptr);
    t_memory_page* page = find_memory_page(block);
    if (page == NULL) {
        return;
    }

}