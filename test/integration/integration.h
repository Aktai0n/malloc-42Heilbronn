#pragma once


void test_with_gnl(const char* infile, const char* outfile);

char* get_next_line(int fd);

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 42
#endif
