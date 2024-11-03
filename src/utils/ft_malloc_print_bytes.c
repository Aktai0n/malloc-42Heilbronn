
#include "utils.h"
#include "libft.h"

void ft_malloc_print_bytes(int fd, void* ptr, size_t size) {
    uint8_t* bytes = (uint8_t*)ptr;
    if (size != 0) {
        ft_dprintf(fd, "%b", *bytes);
    }
    for (size_t i = 1; i < size; ++i) {
        ft_dprintf(fd, " %b", bytes[i]);
    }
}
