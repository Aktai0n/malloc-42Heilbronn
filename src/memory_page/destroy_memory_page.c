
#include <stdbool.h>

#include "memory_page.h"

bool destroy_memory_page(t_memory_page* page) {
    return call_munmap(page);
}