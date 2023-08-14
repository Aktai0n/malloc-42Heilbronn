
#include <stdlib.h>
#include <malloc.h>
#include <stdint.h>

#if 0

#define ARRAY_SIZE (size_t)100

static void free_array(void** array) {
    for (size_t i = 0; array[i] != NULL; ++i) {
        free(array[i]);
    }
    free(array);
}

static void** alloc_array(size_t num_items, size_t item_size) {
    void** array = calloc(num_items + 1, sizeof(void*));
    if (array == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < num_items; ++i) {
        array[i] = calloc(1, item_size);
        if (array[i] == NULL) {
            free_array(array);
            return NULL;
        }
    }
    return array;
}

static void test_actual_allocated_size(void) {
    const size_t n = 100;
    for (size_t i = 0; i < n; ++i) {
        size_t size = i * 100;
        void* ptr = malloc(size);
        // printf("size = %4zu, allocated size = %zu\n", size, malloc_usable_size(ptr));
        free(ptr);
    }
}

int main(void) {
    void* ptr = malloc(PTRDIFF_MAX - 1);

    test_actual_allocated_size();

    malloc_info(0, stdout);
    struct mallinfo2 info =  mallinfo2();

    return 0;
}

#endif