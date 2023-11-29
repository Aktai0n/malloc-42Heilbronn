
#include <stdbool.h>

#include "memory_page.h"

bool destroy_memory_page(void* ptr) {
    t_memory_page* page = (char*)ptr - sizeof(*page);
    return call_munmap(page);
}