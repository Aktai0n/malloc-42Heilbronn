
#include <limits.h>
#include <unistd.h>

#include "libft.h"

ssize_t ft_putint_base_fd(ssize_t number, uint8_t base, int fd) {
    ssize_t ret = 0;
    size_t unsigned_number = 0;
    if (number < 0) {
        ret = write(fd, "-", 1);
        unsigned_number = (size_t)-number;
    } else {
        unsigned_number = (size_t)number;
    }
    ret += ft_putuint_base_fd(unsigned_number, base, fd);
    return ret;
}
