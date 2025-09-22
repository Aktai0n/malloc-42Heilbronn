
#include <stdarg.h>
#include <unistd.h>
#include <errno.h>

#include "libft.h"

static ssize_t print_addr_(void* ptr, int fd) {
    size_t addr = (size_t)ptr;
    ssize_t ret = write(fd, "0x", 2);
    ret += ft_putuint_base_fd(addr, 16, fd);
    return ret;
}

static ssize_t print_string_(const char* s, int fd) {
    if (s == NULL) {
        s = "(null)";
        return write(fd, s, 6);
    }
    return ft_putstr_fd(s, fd);
}

static ssize_t resolve_format_specifiers(
    const char* format,
    va_list args,
    int fd
) {
    int temp = 0;
    switch (*format) {
    case '%':
        return write(fd, "%", 1);
    case 'b':
        return ft_putbyte_hex_fd((uint8_t)va_arg(args, int), fd);
    case 'c':
        temp = va_arg(args, int);
        return write(fd, &temp, 1);
    case 's':
        return print_string_(va_arg(args, char*), fd);
    case 'd':
    case 'i':
        return ft_putint_base_fd(va_arg(args, int), 10, fd);
    case 'u':
        return ft_putuint_base_fd(va_arg(args, size_t), 10, fd);
    case 'x':
    case 'X':
        return ft_putuint_base_fd(va_arg(args, size_t), 16, fd);
    case 'p':
        return print_addr_(va_arg(args, void*), fd);
    default:
        errno = EINVAL;
        return -1;
    }
}

int ft_vdprintf(int fd, const char* format, va_list args) {
    ssize_t ret = 0;
    char buf[0x100];
    size_t i = 0;
    for (; *format != '\0'; ++format) {
        if (*format == '%') {
            ret += write(fd, buf, i);
            ret += resolve_format_specifiers(++format, args, fd);
            i = 0;
        } else {
            buf[i] = *format;
            ++i;
        }
        if (i == sizeof(buf)) {
            ret += write(fd, buf, sizeof(buf));
            i = 0;
        }
    }
    if (i != 0) {
        ret += write(fd, buf, i);
    }
    return (int)ret;
}

int ft_dprintf(int fd, const char* format, ...) {
    int ret = 0;
    va_list args;
    va_start(args, format);
    ret = ft_vdprintf(fd, format, args);
    va_end(args);
    return ret;
}

int ft_printf(const char* format, ...) {
    int ret = 0;
    va_list args;
    va_start(args, format);
    ret = ft_vdprintf(STDIN_FILENO, format, args);
    va_end(args);
    return ret;
}
