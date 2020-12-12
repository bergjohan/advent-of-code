#include <ctype.h>
#include <stdbool.h>

#include "common.h"
#include "stretchy_buffer.h"

typedef enum Token_kind {
    TOKEN_UNKNOWN,
    TOKEN_EOF,
    TOKEN_COMMA,
    TOKEN_PERIOD,
    TOKEN_NUMBER,
    TOKEN_BAG,
    TOKEN_CONTAIN,
    TOKEN_NO,
    TOKEN_OTHER,
} Token_kind;

typedef struct Token {
    const char *start;
    size_t size;
    Token_kind kind;
} Token;

const char *buffer;
Token token;
Map map;

void next_token(void) {
next:
    token.start = buffer;

    switch (*buffer) {
    case 0:
        token.kind = TOKEN_EOF;
        break;
    case '\n':
    case ' ':
        while (isspace(*buffer)) {
            buffer++;
        }
        goto next;
    case ',':
        token.kind = TOKEN_COMMA;
        buffer++;
        break;
    case '.':
        token.kind = TOKEN_PERIOD;
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
        token.kind = TOKEN_NUMBER;
        while (isdigit(*buffer)) {
            buffer++;
        }
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
        die("unexpected token\n");
    }
}

void init_tokens(void) {
    map_insert(&map, "bag", 3, (void *)TOKEN_BAG);
    map_insert(&map, "bags", 4, (void *)TOKEN_BAG);
    map_insert(&map, "contain", 7, (void *)TOKEN_CONTAIN);
    map_insert(&map, "no", 2, (void *)TOKEN_NO);
    map_insert(&map, "other", 5, (void *)TOKEN_OTHER);
}

typedef struct Edge {
    struct Node *node;
    int num;
} Edge;

typedef struct Node {
    Edge *edges;
} Node;

Map nodes;
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
    Node *node2 = map_find(&nodes, key2, size2);
    Edge edge = {node2, num};
    sb_push(node1->edges, edge);
}

void build_graph(void) {
    next_token();
    while (token.kind != TOKEN_EOF) {
        const char *start = token.start;
        size_t size = token.size;
        add_node(token.start, token.size);
        next_token();
        expect(TOKEN_BAG);
        expect(TOKEN_CONTAIN);
        if (token.kind == TOKEN_NO) {
            next_token();
            expect(TOKEN_OTHER);
            expect(TOKEN_BAG);
            expect(TOKEN_PERIOD);
            continue;
        }
        int num = atoi(token.start);
        expect(TOKEN_NUMBER);
        add_node(token.start, token.size);
        add_edge(start, size, token.start, token.size, num);
        next_token();
        expect(TOKEN_BAG);
        while (token.kind == TOKEN_COMMA) {
            next_token();
            int num = atoi(token.start);
            expect(TOKEN_NUMBER);
            add_node(token.start, token.size);
            add_edge(start, size, token.start, token.size, num);
            next_token();
            expect(TOKEN_BAG);
        }
        expect(TOKEN_PERIOD);
    }

    shiny_gold = map_find(&nodes, "shiny gold", 10);
}

bool contain_shiny_gold(Node *node) {
    for (int i = 0; i < sb_count(node->edges); i++) {
        Node *n = node->edges[i].node;
        if (n == shiny_gold) {
            return true;
        }
        if (contain_shiny_gold(n)) {
            return true;
        }
    }
    return false;
}

int part1(void) {
    int count = 0;
    for (int i = 0; i < MAP_SIZE; i++) {
        if (nodes.keys[i][0] != '\0') {
            if (contain_shiny_gold(nodes.values[i])) {
                count++;
            }
        }
    }
    return count;
}

int count_bags(Node *node) {
    int count = 0;
    for (int i = 0; i < sb_count(node->edges); i++) {
        Node *n = node->edges[i].node;
        count += node->edges[i].num + node->edges[i].num * count_bags(n);
    }
    return count;
}

int part2(void) {
    return count_bags(shiny_gold);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s [FILE]\n", argv[0]);
        exit(1);
    }
    char *input = read_input(argv[1]);
    buffer = input;
    init_tokens();
    build_graph();
    printf("%d\n", part1());
    printf("%d\n", part2());
    free(input);
}
