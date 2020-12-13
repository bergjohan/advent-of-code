#pragma once

#include <inttypes.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

void die(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    fprintf(stderr, "fatal: ");
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    exit(1);
}

uint64_t timer(void) {
    struct timespec tp;
    clock_gettime(CLOCK_MONOTONIC, &tp);
    return ((uint64_t)tp.tv_sec * 1000) + ((uint64_t)tp.tv_nsec / 1000000);
}

#define MAP_SIZE 1024

typedef struct String {
    char str[32];
    size_t len;
} String;

typedef struct Map {
    String keys[MAP_SIZE];
    void *values[MAP_SIZE];
} Map;

uint32_t hash(const char *str, size_t size) {
    const unsigned char *s = (const unsigned char *)str;
    uint32_t hash = 0x811c9dc5;

    for (size_t i = 0; i < size; i++) {
        hash ^= s[i];
        hash *= 0x01000193;
    }
    return hash;
}

void map_insert_range(Map *map, const char *start, const char *end,
                      void *value) {
    size_t len = (size_t)(end - start);
    uint32_t i = hash(start, len);

    for (;;) {
        i &= MAP_SIZE - 1;
        if (map->keys[i].str[0] == 0) {
            map->keys[i].len = len;
            memcpy(map->keys[i].str, start, len);
            map->keys[i].str[len] = 0;
            map->values[i] = value;
            return;
        }
        i++;
    }
}

void map_insert(Map *map, const char *key, void *value) {
    map_insert_range(map, key, key + strlen(key), value);
}

void *map_find_range(Map *map, const char *start, const char *end) {
    size_t len = (size_t)(end - start);
    uint32_t i = hash(start, len);

    for (;;) {
        i &= MAP_SIZE - 1;
        if (map->keys[i].len == len &&
            strncmp(map->keys[i].str, start, len) == 0) {
            return map->values[i];
        } else if (map->keys[i].str[0] == 0) {
            return NULL;
        }
        i++;
    }
}

void *map_find(Map *map, const char *key) {
    return map_find_range(map, key, key + strlen(key));
}

char *read_input(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        die("fopen() failed\n");
    }
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    rewind(fp);
    char *ret = malloc((size_t)size + 1);
    if (!ret) {
        die("malloc() failed\n");
    }
    fread(ret, 1, (size_t)size, fp);
    ret[size] = 0;
    fclose(fp);
    return ret;
}
