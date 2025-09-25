
#include "utils.h"

bool ft_is_in_region(
    const void* ptr,
    const void* region_start,
    size_t region_size
) {
    return (size_t)ptr >= (size_t)region_start &&
        (size_t)ptr < (size_t)region_start + region_size;
}
