
#include <errno.h>

#include "memory_page/memory_page.h"
#include "alloc_block/alloc_block.h"


size_t available_memory_size(void* ptr) {
    t_alloc_block* block = (t_alloc_block*)ptr - 1;
    t_memory_page* page = find_memory_page(block);
    if (page == NULL) {
        errno = EINVAL;
        return 0;
    }
    return block->size;
}
