
#include "large.h"
#include "page/large_page.h"

t_large_page* allocate_large(size_t size,t_large_page** pages) {
    return create_large_page(size, 0, pages);
}

t_large_page* reallocate_large(
    t_large_page* page,
    const size_t size,
    t_large_page** pages
) {
    return realloc_large_page(page, size, pages);
}

bool deallocate_large(t_large_page* page, t_large_page** pages) {
    return destroy_large_page(page, pages);
}
