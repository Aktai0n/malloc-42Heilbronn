
#include "utils.h"

void ft_malloc_bzero(void* start, void* end) {
    size_t* ptr = (size_t*)start;
    while (ptr < end) {
        *ptr = 0;
        ++ptr;
    }
}
