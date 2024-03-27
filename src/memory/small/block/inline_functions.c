
#include "small_block.h"

extern inline t_small_block* get_small_block(void* ptr);
extern inline void* get_small_block_data(t_small_block* block);
extern inline t_small_block* get_next_small_block(t_small_block* block);
extern inline t_small_block* get_prev_small_block(t_small_block* block);
extern inline void copy_small_block_data(
    t_small_block* src,
    t_small_block* dst
);