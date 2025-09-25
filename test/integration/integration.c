#include "integration.h"
#include "ft_malloc.h"
#include "libft.h"

#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

void test_with_gnl(const char* infile, const char* outfile) {
    int fd = open(infile, O_RDONLY);
    if (fd < 0) {
        ft_dprintf(STDERR_FILENO, "open() failed for %s: %s\n", infile, strerror(errno));
        return;
    }
    int write_fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0664);
    if (write_fd < 0) {
        close(fd);
        ft_dprintf(STDERR_FILENO, "open() failed for %s: %s\n", outfile, strerror(errno));
        return;
    }

    char** lines = calloc(10, sizeof(char*));
    if (lines == NULL) {
        return;
    }
    size_t i = 0;
    char* str = get_next_line(fd);
    for (;str != NULL; ++i) {
        // ft_printf("%u: %s", i, str);
        lines = realloc(lines, (i + 1) * sizeof(char*));
        if (lines == NULL) {
            break;
        }
        lines[i] = str;
        check_heap();
        str = get_next_line(fd);
        check_heap();
    }
    show_alloc_mem_ex();
    show_alloc_mem();

    lines[i + 1] = NULL;
    for (i = 0; lines[i] != NULL; ++i) {
        ft_dprintf(write_fd, "%u: %s", i + 1, lines[i]);
        free(lines[i]);
        check_heap();
    }
    free(lines);
    close(fd);
    close(write_fd);
}
