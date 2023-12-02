
#include <stddef.h>
#include <sys/types.h>
#include <stdint.h>

ssize_t ft_putstr(const char* str);
ssize_t ft_putstr_fd(const char* str, int fd);
ssize_t ft_putstr_color(const char* color, const char* str);

ssize_t ft_putbyte_hex(uint8_t byte);

size_t ft_strlen(const char* str);

ssize_t ft_putuint_base(size_t number, uint8_t base);