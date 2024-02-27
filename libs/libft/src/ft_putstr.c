
#include <unistd.h>

#include "libft.h"

ssize_t ft_putstr(const char* str) {
    size_t len = ft_strlen(str);
    return write(STDOUT_FILENO, str, len);
}
