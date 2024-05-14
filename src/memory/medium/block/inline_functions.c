
#include "medium_block.h"

extern inline void* get_medium_block_data(t_medium_block* block);
extern inline t_medium_block* get_medium_block(void* ptr);
extern inline void* get_medium_block_data_end(t_medium_block* block);
extern inline void copy_medium_block_data(
    t_medium_block* src,
    t_medium_block* dst
);
