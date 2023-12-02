
#include <unistd.h>

#include "libft.h"

ssize_t ft_putstr_fd(const char* str, int fd) {
    size_t len = ft_strlen(str);
    return write(fd, str, len);
}