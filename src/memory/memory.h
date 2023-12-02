#pragma once

#include <stddef.h>
#include <stdbool.h>

void* allocate_memory(size_t requested_block_size);

bool release_memory(void* ptr);
