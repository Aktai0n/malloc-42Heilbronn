
#include "utils.h"

void ft_malloc_bzero(void* start, void* end) {
    size_t* ptr = (size_t*)start;
    while ((size_t)ptr < (size_t)end) {
        *ptr = 0;
        ++ptr;
    }
}
