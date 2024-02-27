
#include <unistd.h>

#include "colors.h"
#include "libft.h"

ssize_t ft_putstr_color(const char* color, const char* str) {
    size_t len = ft_strlen(color);
    ssize_t ret = write(STDOUT_FILENO, color, len);
    len = ft_strlen(str);
    ret += write(STDOUT_FILENO, str, len);
    ret += write(STDOUT_FILENO, RESET_COLOR, sizeof(RESET_COLOR));
    return ret;
}
