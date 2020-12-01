#pragma once

#include <inttypes.h>
#include <time.h>

uint64_t aoc_clock(void) {
    struct timespec tp;
    clock_gettime(CLOCK_MONOTONIC, &tp);
    return ((uint64_t)tp.tv_sec * 1000) + ((uint64_t)tp.tv_nsec / 1000000);
}
