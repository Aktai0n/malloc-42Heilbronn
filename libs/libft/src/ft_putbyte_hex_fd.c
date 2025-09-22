
#include <unistd.h>

#include "libft.h"

static ssize_t ft_putnibble_hex_(uint8_t nibble, int fd) {
    const char* charset = "0123456789ABCDEF";
    if (nibble <= 16) {
        return write(fd, charset + nibble, 1);
    }
    return 0;
}

ssize_t ft_putbyte_hex_fd(uint8_t byte, int fd) {
    uint8_t upper_nibble = byte / 16;
    uint8_t lower_nibble = byte % 16;
    ssize_t ret = ft_putnibble_hex_(upper_nibble, fd);
    ret += ft_putnibble_hex_(lower_nibble, fd);
    return ret;
}
