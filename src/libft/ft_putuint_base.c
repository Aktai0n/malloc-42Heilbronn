
#include <errno.h>
#include <unistd.h>

#include "libft.h"

#include <stdio.h>

static const char* pick_charset_(uint8_t base) {
    switch (base) {
    case 8:
        return "01234567";
    case 10:
        return "0123456789";
    case 16:
        return "0123456789ABCDEF";
    default:
        return NULL;
    }
}

static char* number_to_string_(
    const size_t number,
    const uint8_t base,
    const char* charset,
    char* buf
) {
    uint8_t digit = number % base;
    if (number >= base) {
        buf = number_to_string_(number / base, base, charset, buf);
    }
    *buf = charset[digit];
    return buf + 1;
}

ssize_t ft_putuint_base(size_t number, uint8_t base) {
    char buf[0x40] = { 0 };
    const char* charset = pick_charset_(base);
    if (charset == NULL) {
        errno = EINVAL;
        return -1;
    }
    number_to_string_(number, base, charset, buf);
    return write(STDOUT_FILENO, buf, ft_strlen(buf));
}