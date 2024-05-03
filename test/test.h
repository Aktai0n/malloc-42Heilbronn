#pragma once

#include "ft_malloc_internal.h"

void printf_test(void);

void test_large_page(struct s_heap* heap);

void test_small_page(struct s_heap* heap);

void test_small_block(struct s_heap* heap);