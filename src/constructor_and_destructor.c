#include <pthread.h>

#include "ft_malloc_internal.h"
#include "platform_specific.h"
#include "memory/small/page/small_page.h"
#include "memory/medium/page/medium_page.h"
#include "memory/large/page/large_page.h"
#include "defines.h"

#include "libft.h"

// initialize a static mutex
pthread_mutex_t g_alloc_mutex = PTHREAD_MUTEX_INITIALIZER;

struct s_heap g_heap = { 0 };

REGISTER_CONSTRUCTOR(ft_malloc_constructor);
REGISTER_DESTRUCTOR(ft_malloc_destructor);

#include <signal.h>
#include <stdlib.h>

void handle_sigsegv(int signo) {
    show_alloc_mem();
    // ft_printf("Segmentation fault\n");
    exit(1);
}

static void ft_malloc_constructor(void) {

    // ft_printf("constructor called\n");
    t_small_page* small_page = create_small_page(
        TINY_PAGE_SIZE,
        0,
        &g_heap.small_pages
    );
    if (small_page == NULL) {
        g_heap.small_pages = NULL;
    }
    t_medium_page* medium_page = create_medium_page(
        SMALL_PAGE_SIZE,
        0,
        &g_heap.medium_pages
    );
    if (medium_page == NULL) {
        g_heap.medium_pages = NULL;
    }
    g_heap.large_pages = NULL;
    // signal(SIGSEGV, handle_sigsegv);
}

static void ft_malloc_destructor(void) {
    // TODO: Check if pages are still in use
    for (t_small_page* page = g_heap.small_pages; page != NULL;) {
        t_small_page* next = page->next;
        destroy_small_page(page, &g_heap.small_pages);
        page = next;
    }
    for (t_medium_page* page = g_heap.medium_pages; page != NULL;) {
        t_medium_page* next = page->next;
        destroy_medium_page(page, &g_heap.medium_pages);
        page = next;
    }
    for (t_large_page* page = g_heap.large_pages; page != NULL;) {
        t_large_page* next = page->next;
        destroy_large_page(page, &g_heap.large_pages);
        page = next;
    }
    // ft_printf("destructor called\n");
}
