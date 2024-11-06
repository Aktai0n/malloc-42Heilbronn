#include "integration.h"
#include "ft_malloc.h"
#include "libft.h"

#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

void test_with_gnl(const char* file) {
    int fd = open(file, O_RDONLY);
    if (fd < 0) {
        ft_dprintf(STDERR_FILENO, "open() failed: %s\n", strerror(errno));
        return;
    }
    char* str = NULL;
    char** lines = calloc(10, sizeof(char*));
    if (lines == NULL) {
        return;
    }
    for (size_t i = 1;(str = get_next_line(fd)); ++i) {
        ft_printf("%u: %s", i, str);
        lines = realloc(lines, i * sizeof(char*));
        if (lines == NULL) {
            return;
        }
        // show_alloc_mem();
        lines[i] = str;
    }
    close(fd);
    show_alloc_mem();
}
