


#include "malloc.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(void) {
    char temp[] = "teststring";

    char* ptr = malloc(sizeof(temp));
    if (ptr == NULL) {
        perror("malloc");
        return 1;
    }
    printf("getpagesize = %d\n", getpagesize());
    strcpy(ptr, temp);
    printf("ptr: %s, %p\n", ptr, (void*)ptr);
    free(ptr);
    return 0;
}