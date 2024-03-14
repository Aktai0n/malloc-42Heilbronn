
#include "utils.h"

void ft_malloc_memcpy(void* src, void* dst, size_t size) {
    size_t* src_ptr = (size_t*)src;
    size_t* dst_ptr = (size_t*)dst;
    for (size_t i = 0; i < size; i += sizeof(size_t)) {
        dst_ptr[i] = src_ptr[i];
    }
}