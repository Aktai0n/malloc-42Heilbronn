
#include <unistd.h>

#include "libft.h"

ssize_t ft_putstr(const char* str) {
    return ft_putstr_fd(str, STDOUT_FILENO);
}
