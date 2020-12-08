#include <ctype.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "aoc_common.h"
#include "stretchy_buffer.h"

typedef enum Token_kind {
    TOK_UNKNOWN,
    TOK_EOF,
    TOK_COMMA,
    TOK_PERIOD,
    TOK_NUMBER,
    TOK_BAG,
    TOK_CONTAIN,
    TOK_NO,
    TOK_OTHER,
} Token_kind;

typedef struct Token {
    const char *start;
    size_t size;
    Token_kind kind;
} Token;

#define MAP_SIZE 1024
typedef char Key[32];

typedef struct Map {
    Key keys[MAP_SIZE];
    void *values[MAP_SIZE];
} Map;

const char *buffer;
Token token;
Map map;

uint32_t hash(const char *str, size_t size) {
    const unsigned char *s = (const unsigned char *)str;
    uint32_t hash = 0x811c9dc5;

    for (size_t i = 0; i < size; i++) {
        hash ^= s[i];
        hash *= 0x01000193;
    }
    return hash;
}

void map_insert(Map *map, const char *key, size_t size, void *value) {
    uint32_t i = hash(key, size);

    for (;;) {
        i &= MAP_SIZE - 1;
        if (map->keys[i][0] == 0) {
            memcpy(map->keys[i], key, size);
            map->values[i] = value;
            return;
        }
        i++;
    }
}

void *map_find(Map *map, const char *key, size_t size) {
    uint32_t i = hash(key, size);

    for (;;) {
        i &= MAP_SIZE - 1;
        if (strlen(map->keys[i]) == size &&
            strncmp(key, map->keys[i], size) == 0) {
            return map->values[i];
        } else if (strlen(map->keys[i]) == 0) {
            return NULL;
        }
        i++;
    }
}

void init_tokens(void) {
    map_insert(&map, "bag", 3, (void *)TOK_BAG);
    map_insert(&map, "bags", 4, (void *)TOK_BAG);
    map_insert(&map, "contain", 7, (void *)TOK_CONTAIN);
    map_insert(&map, "no", 2, (void *)TOK_NO);
    map_insert(&map, "other", 5, (void *)TOK_OTHER);
}

void next_token(void) {
next:
    token.start = buffer;

    switch (*buffer) {
    case 0:
        token.kind = TOK_EOF;
        break;
    case ' ':
    case '\n':
        buffer++;
        goto next;
    case ',':
        token.kind = TOK_COMMA;
        buffer++;
        break;
    case '.':
        token.kind = TOK_PERIOD;
        buffer++;
        break;
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        while (isdigit(*buffer)) {
            buffer++;
        }
        token.kind = TOK_NUMBER;
        break;
    default:
        while (isalpha(*buffer)) {
            buffer++;
        }
        void *ret = map_find(&map, token.start, (size_t)(buffer - token.start));
        if (!ret) {
            buffer++;
            while (isalpha(*buffer)) {
                buffer++;
            }
        }
        token.kind = (Token_kind)(intptr_t)ret;
        break;
    }

    token.size = (size_t)(buffer - token.start);
}

void expect(Token_kind kind) {
    if (token.kind == kind) {
        next_token();
    } else {
        aoc_die("unexpected token\n");
    }
}

Map nodes;

typedef struct Edge {
    const char *key;
    size_t size;
    int num;
} Edge;

typedef struct Node {
    Edge *edges;
} Node;

Node *shiny_gold;

void add_node(const char *key, size_t size) {
    if (!map_find(&nodes, key, size)) {
        Node *node = malloc(sizeof(*node));
        node->edges = NULL;
        map_insert(&nodes, key, size, node);
    }
}

void add_edge(const char *key1, size_t size1, const char *key2, size_t size2,
              int num) {
    Node *node1 = map_find(&nodes, key1, size1);
    Edge edge = {key2, size2, num};
    sb_push(node1->edges, edge);
}

bool contain_shiny_gold(Node *node) {
    for (int i = 0; i < sb_count(node->edges); i++) {
        Node *n = map_find(&nodes, node->edges[i].key, node->edges[i].size);
        if (n == shiny_gold) {
            return true;
        }
        if (contain_shiny_gold(n)) {
            return true;
        }
    }
    return false;
}

int count_bags(Node *node) {
    int count = 0;
    for (int i = 0; i < sb_count(node->edges); i++) {
        Node *n = map_find(&nodes, node->edges[i].key, node->edges[i].size);
        count += node->edges[i].num + node->edges[i].num * count_bags(n);
    }
    return count;
}

void solution(void) {
    next_token();
    while (token.kind != TOK_EOF) {
        const char *start = token.start;
        size_t size = token.size;
        add_node(token.start, token.size);
        next_token();
        expect(TOK_BAG);
        expect(TOK_CONTAIN);
        if (token.kind == TOK_NO) {
            next_token();
            expect(TOK_OTHER);
            expect(TOK_BAG);
            expect(TOK_PERIOD);
            continue;
        }
        int num = atoi(token.start);
        expect(TOK_NUMBER);
        add_node(token.start, token.size);
        add_edge(start, size, token.start, token.size, num);
        next_token();
        expect(TOK_BAG);
        while (token.kind == TOK_COMMA) {
            next_token();
            int num = atoi(token.start);
            expect(TOK_NUMBER);
            add_node(token.start, token.size);
            add_edge(start, size, token.start, token.size, num);
            next_token();
            expect(TOK_BAG);
        }
        expect(TOK_PERIOD);
    }

    shiny_gold = map_find(&nodes, "shiny gold", 10);
    int count = 0;
    for (int i = 0; i < MAP_SIZE; i++) {
        if (nodes.keys[i][0] != '\0') {
            if (contain_shiny_gold(nodes.values[i])) {
                count++;
            }
        }
    }
    printf("%d\n", count);
    printf("%d\n", count_bags(shiny_gold));
}

int main(void) {
    char *input = aoc_read_input("input07.txt");
    buffer = input;
    init_tokens();
    solution();
    free(input);
}
