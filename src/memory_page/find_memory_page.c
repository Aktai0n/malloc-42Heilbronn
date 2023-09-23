

#include "memory_page.h"
#include "defines.h"

enum e_memory_page find_page_type(size_t size) {
    if (size <= TINY_ALLOC_BLOCK_SIZE) {
        return TINY_PAGE;
    } else if (size <= SMALL_ALLOC_BLOCK_SIZE) {
        return SMALL_PAGE;
    } else {
        return LARGE_PAGE;
    }
}