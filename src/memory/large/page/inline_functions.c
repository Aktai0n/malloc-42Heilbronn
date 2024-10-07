
#include "large_page.h"

extern inline size_t get_large_page_data_size(const t_large_page* page);
extern inline void* get_large_page_data(t_large_page* page);
extern inline t_large_page_end* get_large_page_end(t_large_page* page);
extern inline bool large_page_is_corrupted(t_large_page* page);
