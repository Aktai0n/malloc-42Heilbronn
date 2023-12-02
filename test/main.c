
#include "defines.h"

#include "ft_malloc.h"
#include "../src/libft/libft.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(void) {
    char temp[] = "teststring";
    ft_putuint_base(SIZE_MAX, 10);
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