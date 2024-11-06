#include "integration.h"
#include "ft_malloc.h"


#include <stdint.h>
#include <unistd.h>
#include <stdbool.h>

static size_t ft_strlen(const char* str) {
    size_t i = 0;

    if (str == NULL) {
        return 0;
    }
    while (str[i] != '\0') {
        ++i;
    }
    return i;
}

static char* ft_strcat(char* s1, size_t s1_len, const char* s2, size_t s2_len) {
    s1 = realloc(s1, s1_len + s2_len + 1);
    if (s1 == NULL) {
        return NULL;
    }
    for (size_t i = 0; i < s2_len; ++i) {
        s1[s1_len + i] = s2[i];
    }
    s1[s1_len + s2_len] = '\0';
    return s1;
}

static bool split_at_delim(char* line, size_t line_len, char delim, char** rest) {
    size_t i;

    i = 0;
    while (i < line_len) {
        if (line[i] == delim) {
            *rest = ft_strcat(NULL, 0, line + i + 1, line_len - i);
            line[i + 1] = '\0';
            return true;
        }
        ++i;
    }
    return false;
}

static char* read_file(int fd, char** rest) {
    char buffer[BUFFER_SIZE + 1];
    char* line;
    size_t line_len;
    ssize_t read_ret;

    line = *rest;
    line_len = ft_strlen(line);
    while (!split_at_delim(line, line_len, '\n', rest)) {
        read_ret = read(fd, buffer, BUFFER_SIZE);
        if (read_ret <= 0) {
            break;
        }
        buffer[read_ret] = '\0';
        line = ft_strcat(line, line_len, buffer, (size_t)read_ret);
        line_len += (size_t)read_ret;
        if (line == NULL) {
            return NULL;
        }
    }
    return line;
}

char* get_next_line(int fd) {
    static char* rest = NULL;
    char* line;

    if (fd < 0) {
        return NULL;
    }
    line = read_file(fd, &rest);
    if (line == rest) {
        rest = NULL;
    }
    return line;
}

