#pragma once

#include <inttypes.h>
#include <stdlib.h>
#include <time.h>

uint64_t aoc_clock(void) {
    struct timespec tp;
    clock_gettime(CLOCK_MONOTONIC, &tp);
    return ((uint64_t)tp.tv_sec * 1000) + ((uint64_t)tp.tv_nsec / 1000000);
}

char *read_input(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "fopen() failed\n");
        exit(1);
    }
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    rewind(fp);
    char *ret = malloc((size_t)size + 1);
    fread(ret, 1, (size_t)size, fp);
    ret[size] = '\0';
    fclose(fp);
    return ret;
}
