
#if 1

#include "defines.h"

#include "ft_malloc.h"
#include "ft_malloc_internal.h"
#include "libft.h"
#include "test.h"
#include "colors.h"
#include "unit/unit.h"
#include "integration/integration.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

static char* get_abs_path_(const char* path) {
    char buf[PATH_MAX] = {0};
    if (getcwd(buf, sizeof(buf)) == NULL) {
        return NULL;
    }
    if (strncmp("./", path, 2) == 0) {
        path = path + 2;
    }
    size_t total_len = strlen(path) + strlen(buf);
    char* full_path = malloc(total_len + 1);
    if (full_path == NULL) {
        return NULL;
    }
    full_path[total_len] = '\0';
    strcat(full_path, buf);
    strcat(full_path, "/");
    strcat(full_path, path);
    return full_path;
}

static void execute_specific_tests_(int argc, char** argv, struct s_heap* heap) {

    for (int i = 1; i < argc; ++i) {
        const char* str = argv[i];
        if (strcmp(str, "l") == 0 || strcmp(str, "large") == 0) {
            test_large_page(heap);
        } else if (strcmp(str, "s") == 0 || strcmp(str, "small") == 0) {
            test_small_page(heap);
            test_small_block(heap);
        } else if (strcmp(str, "m") == 0 || strcmp(str, "medium") == 0) {
            test_medium_page(heap);
            test_medium_block(heap);
        } else if (strcmp(str, "i") == 0 || strcmp(str, "integration") == 0) {
            char* file = get_abs_path_("./test/integration/test_dracula.txt");
            // const char* file = "/Users/skienzle/42Projects/private_github_repos/malloc-42Heilbronn/test/integration/test_dracula.txt";
            test_with_gnl(file);
            free(file);
        }
    }
}

int main(int argc, char** argv) {
    struct s_heap heap = {
        .small_pages = NULL,
        .medium_pages = NULL,
        .large_pages= NULL
    };
    if (argc != 1) {
        execute_specific_tests_(argc, argv, &heap);
        return 0;
    }

    ft_putstr_color(BOLD_INTENSE_YELLOW_COLOR, "------------ Start Unit Tests ------------\n\n");
    test_small_page(&heap);
    test_small_block(&heap);
    show_alloc_mem_ex();
    test_medium_page(&heap);
    test_medium_block(&heap);
    test_large_page(&heap);
    ft_putstr_color(BOLD_INTENSE_YELLOW_COLOR, "------------ End Unit Tests ------------\n\n");
    return 0;
}

#else

#include <stdlib.h>

int      main(void)
{
    int   i;
    char  *addr;

    i = 0; 
    while (i < 1024) 
    {
        i++;
    } 
    return (0); 
}

#endif
