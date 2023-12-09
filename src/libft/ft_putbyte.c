
#include <unistd.h>

#include "libft.h"

static ssize_t ft_putnibble_hex_(uint8_t nibble) {
    const char* charset = "0123456789ABCDEF";
    if (nibble <= 16) {
        return write(STDOUT_FILENO, charset + nibble, 1);
    }
    return 0;
}

ssize_t ft_putbyte_hex(uint8_t byte) {
    uint8_t upper_nibble = byte / 16;
    uint8_t lower_nibble = byte % 16;
    ssize_t ret = ft_putnibble_hex_(upper_nibble);
    ret += ft_putnibble_hex_(lower_nibble);
    return ret;
}