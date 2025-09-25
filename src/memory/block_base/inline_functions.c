
#include "block_base.h"

extern inline size_t get_block_size(t_block_base block);
extern inline bool is_allocated(t_block_base block);
extern inline bool is_last_block(t_block_base block);
extern inline void set_block_flag(
    t_block_base* block,
    size_t flag,
    bool state
);
extern inline void set_block_size(t_block_base* block, const size_t size);
