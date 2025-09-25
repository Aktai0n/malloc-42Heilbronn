
// #define TEST_SWITCH 0

#ifndef TEST_SWITCH

#include "defines.h"

#include "ft_malloc.h"
#include "ft_malloc_internal.h"
#include "libft.h"
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
            test_with_gnl(file, "./integration_output.txt");
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
    // show_alloc_mem_ex();
    test_medium_page(&heap);
    test_medium_block(&heap);
    test_large_page(&heap);
    ft_putstr_color(BOLD_INTENSE_YELLOW_COLOR, "------------ End Unit Tests ------------\n\n");
    return 0;
}


#elif TEST_SWITCH == 0

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

#elif TEST_SWITCH == 1

#include <stdlib.h>

int      main(void)
{
    int   i;
    char  *addr;

    i = 0;
    while (i < 1024)
    {
        addr = (char*)malloc(1024);
        addr[0] = 42;
        i++;
    }
    return (0);
}

#elif TEST_SWITCH == 2

#include <stdlib.h>

int main(void)
{
    int   i;
    char  *addr;

    i = 0;
    while (i < 1024) 
    {
        addr = (char*)malloc(1024);
        addr[0] = 42;
        free(addr); 
        i++; 
    }
    return (0);
}

#elif TEST_SWITCH == 3

#include <strings.h>
#include <stdlib.h>
#include <unistd.h>

#define M (1024 * 1024)

void print(char *s)
{
    write(1, s, strlen(s));
}

int main()
{ char *addr1; char *addr3;

    addr1 = (char*)malloc(16*M);
    strcpy(addr1, "Bonjour\n");
    print(addr1);
    addr3 = (char*)realloc(addr1, 128*M);
    addr3[127*M] = 42;
    print(addr3);
    return (0);
}

#elif TEST_SWITCH == 4

#include <strings.h>
#include <stdlib.h>
#include <unistd.h>

#define M (1024 * 1024)

void print(char *s)
{
    write(1, s, strlen(s));
}

int     main()
{
    char *addr1;
    char *addr2;
    char *addr3;

    addr1 = (char*)malloc(16*M);
    strcpy(addr1, "Bonjour\n");
    print(addr1);
    addr2 = (char*)malloc(16*M);
    addr3 = (char*)realloc(addr1, 128*M);
    addr3[127*M] = 42;
    print(addr3);
    return (0);
}

#elif TEST_SWITCH == 5

#include <unistd.h>
#include <string.h>
#include <stdlib.h>

void print(char *s)
{
    write(1, s, strlen(s));
}

int main()
{
    char *addr;

    addr = malloc(16);
    free(NULL);
    free((void *)addr + 5);
    if (realloc((void *)addr + 5, 10) == NULL)
        print("Bonjour\n");
}

#elif TEST_SWITCH == 6

#include <stdlib.h>
#include "inc/ft_malloc.h"

int main()
{
    malloc(1024);
    malloc(1024 * 32);
    malloc(1024 * 1024);
    malloc(1024 * 1024 * 16);
    malloc(1024 * 1024 * 128);
    show_alloc_mem(); 
    return (0); 
}

#endif
