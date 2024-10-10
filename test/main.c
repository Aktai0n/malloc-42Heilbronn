
#if 1

#include "defines.h"

#include "ft_malloc.h"
#include "ft_malloc_internal.h"
#include "libft.h"
#include "test.h"
#include "colors.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

static void execute_specific_tests_(int argc, char** argv) {
    struct s_heap heap = {
        .tiny_pages = NULL,
        .small_pages = NULL,
        .large_pages= NULL
    };
    for (int i = 1; i < argc; ++i) {
        const char* str = argv[i];
        if (strcmp(str, "l") == 0 || strcmp(str, "large") == 0) {
            test_large_page(&heap);
        } else if (strcmp(str, "s") == 0 || strcmp(str, "small") == 0) {
            test_small_page(&heap);
            test_small_block(&heap);
        } else if (strcmp(str, "m") == 0 || strcmp(str, "medium") == 0) {
            test_medium_page(&heap);
            test_medium_block(&heap);
        }
    }
}

int main(int argc, char** argv) {
    if (argc != 1) {
        execute_specific_tests_(argc, argv);
        return 0;
    }
    // printf_test();
    void* ptr = malloc(16);
    void* large = malloc(UINT32_MAX);
    // malloc(20);
    ptr = realloc(ptr, 48);
    // show_alloc_mem();
    free(ptr);
    // show_alloc_mem_ex();
    show_alloc_mem();
    // char temp[] = "teststring";
    // ft_putuint_base_fd(SIZE_MAX, 10, STDIN_FILENO);
    
    // char* ptr = malloc(sizeof(temp));
    // if (ptr == NULL) {
    //     perror("malloc");
    //     return 1;
    // }
    // show_alloc_mem_ex();
    // printf("aligned size = %zu\n", ALIGN_ALLOC_SIZE(16));
    // printf("getpagesize = %d\n", getpagesize());
    // printf("tiny page = %zu, tiny block = %zu\n", TINY_PAGE_SIZE, TINY_ALLOC_BLOCK_SIZE);
    // printf("small page = %zu, small block = %zu\n", SMALL_PAGE_SIZE, SMALL_ALLOC_BLOCK_SIZE);
    // strcpy(ptr, temp);
    // printf("ptr: %s, %p\n", ptr, (void*)ptr);
    // free(ptr);
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