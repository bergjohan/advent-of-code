#pragma once

#include <inttypes.h>
#include <stdarg.h>
#include <stdlib.h>
#include <time.h>

void aoc_die(const char *fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    fprintf(stderr, "fatal: ");
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    exit(1);
}

uint64_t aoc_clock(void) {
    struct timespec tp;
    clock_gettime(CLOCK_MONOTONIC, &tp);
    return ((uint64_t)tp.tv_sec * 1000) + ((uint64_t)tp.tv_nsec / 1000000);
}

char *aoc_read_input(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        aoc_die("fopen() failed\n");
    }
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    rewind(fp);
    char *ret = malloc((size_t)size + 1);
    if (!ret) {
        aoc_die("malloc() failed\n");
    }
    fread(ret, 1, (size_t)size, fp);
    ret[size] = '\0';
    fclose(fp);
    return ret;
}
