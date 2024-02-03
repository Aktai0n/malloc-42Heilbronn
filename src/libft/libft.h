#pragma once

#include <stddef.h>
#include <sys/types.h>
#include <stdint.h>

ssize_t ft_putstr(const char* str);
ssize_t ft_putstr_fd(const char* str, int fd);
ssize_t ft_putstr_color(const char* color, const char* str);

ssize_t ft_putbyte_hex(uint8_t byte);

size_t ft_strlen(const char* str);

ssize_t ft_putuint_base_fd(size_t number, uint8_t base, int fd);
ssize_t ft_putint_base_fd(ssize_t number, uint8_t base, int fd);

int ft_printf(const char* format, ...);
int ft_dprintf(int fd, const char* format, ...);
