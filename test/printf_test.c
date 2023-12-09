
#include "defines.h"

#include "ft_malloc.h"
#include "../src/libft/libft.h"


void printf_test(void) {
    ft_printf("hello world!\n");
    ft_printf("%%\n");
    ft_printf("%i\n", -12);
    ft_printf("%s\n", NULL);
    ft_printf("%b\n", 0x46);
}