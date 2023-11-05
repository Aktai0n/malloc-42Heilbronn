
#include "alloc_block.h"

extern inline size_t get_alloc_size(const t_alloc_block* alloc_node);
extern inline bool is_allocated(const t_alloc_block* alloc_node);
extern inline bool is_last_block(const t_alloc_block* block);
extern inline void set_alloc_block_flag(t_alloc_block* block, size_t flag, bool state);
extern inline t_alloc_block* get_next_block_in_memory(t_alloc_block* block);

extern inline t_alloc_block* get_next_alloc(const t_alloc_block* current_node);