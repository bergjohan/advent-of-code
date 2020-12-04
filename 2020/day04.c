#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "aoc_common.h"

typedef enum Token_kind {
    TOK_UNKNOWN,
    TOK_BYR = 0x00000001,
    TOK_IYR = 0x00000010,
    TOK_EYR = 0x00000100,
    TOK_HGT = 0x00001000,
    TOK_HCL = 0x00010000,
    TOK_ECL = 0x00100000,
    TOK_PID = 0x01000000,
    TOK_CID = 0x10000000,
    TOK_CM,
    TOK_IN,
    TOK_COLOR,
    TOK_EOF,
    TOK_NEXT,
    TOK_COLON,
    TOK_NUMBER,
    TOK_HASH
} Token_kind;

#define BITMASK                                                                \
    (TOK_BYR | TOK_IYR | TOK_EYR | TOK_HGT | TOK_HCL | TOK_ECL | TOK_PID)

typedef struct Token {
    const char *start;
    size_t size;
    Token_kind kind;
} Token;

#define MAP_SIZE 32
typedef char Name[16];

typedef struct Map {
    Name names[MAP_SIZE];
    Token_kind kinds[MAP_SIZE];
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

void map_insert(const char *name, Token_kind kind) {
    uint32_t i = hash(name, strlen(name));

    for (;;) {
        i &= MAP_SIZE - 1;
        if (strlen(map.names[i]) == 0) {
            strcpy(map.names[i], name);
            map.kinds[i] = kind;
            return;
        }
        i++;
    }
}

Token_kind map_find(const char *name, size_t size) {
    uint32_t i = hash(name, size);

    for (;;) {
        i &= MAP_SIZE - 1;
        if (strlen(map.names[i]) == size &&
            strncmp(name, map.names[i], size) == 0) {
            return map.kinds[i];
        } else if (strlen(map.names[i]) == 0) {
            return TOK_UNKNOWN;
        }
        i++;
    }
}

void next_token(void) {
next:
    token.start = buffer;

    switch (*buffer) {
    case 0:
        token.kind = TOK_EOF;
        break;
    case '\n':
        buffer++;
        if (*buffer != '\n') {
            goto next;
        }
        token.kind = TOK_NEXT;
        buffer++;
        break;
    case ' ':
        while (isspace(*buffer)) {
            buffer++;
        }
        goto next;
    case ':':
        token.kind = TOK_COLON;
        buffer++;
        break;
    case '#':
        buffer++;
        while (isxdigit(*buffer)) {
            buffer++;
        }
        token.kind = TOK_HASH;
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
        token.kind = map_find(token.start, (size_t)(buffer - token.start));
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

typedef struct Result {
    int part1;
    int part2;
} Result;

Result solve(void) {
    Result res = {0};
    uint32_t bits1 = 0;
    uint32_t bits2 = 0;

    next_token();
    while (token.kind != TOK_EOF) {
        if (token.kind == TOK_NEXT) {
            if ((bits1 & BITMASK) == BITMASK) {
                res.part1++;
            }
            if ((bits2 & BITMASK) == BITMASK) {
                res.part2++;
            }
            bits1 = 0;
            bits2 = 0;
            next_token();
        }

        switch (token.kind) {
        case TOK_BYR:
            next_token();
            expect(TOK_COLON);
            if (token.kind == TOK_NUMBER) {
                int year = atoi(token.start);
                if (year >= 1920 && year <= 2002) {
                    bits2 |= TOK_BYR;
                }
                next_token();
            }
            bits1 |= TOK_BYR;
            break;
        case TOK_IYR:
            next_token();
            expect(TOK_COLON);
            if (token.kind == TOK_NUMBER) {
                int year = atoi(token.start);
                if (year >= 2010 && year <= 2020) {
                    bits2 |= TOK_IYR;
                }
                next_token();
            }
            bits1 |= TOK_IYR;
            break;
        case TOK_EYR:
            next_token();
            expect(TOK_COLON);
            if (token.kind == TOK_NUMBER) {
                int year = atoi(token.start);
                if (year >= 2020 && year <= 2030) {
                    bits2 |= TOK_EYR;
                }
                next_token();
            }
            bits1 |= TOK_EYR;
            break;
        case TOK_HGT:
            next_token();
            expect(TOK_COLON);
            if (token.kind == TOK_NUMBER) {
                int height = atoi(token.start);
                next_token();
                if (token.kind == TOK_CM) {
                    if (height >= 150 && height <= 193) {
                        bits2 |= TOK_HGT;
                    }
                    next_token();
                } else if (token.kind == TOK_IN) {
                    if (height >= 59 && height <= 76) {
                        bits2 |= TOK_HGT;
                    }
                    next_token();
                }
            }
            bits1 |= TOK_HGT;
            break;
        case TOK_HCL:
            next_token();
            expect(TOK_COLON);
            if (token.kind == TOK_HASH && (token.size - 1) == 6) {
                bits2 |= TOK_HCL;
            }
            bits1 |= TOK_HCL;
            break;
        case TOK_ECL:
            next_token();
            expect(TOK_COLON);
            if (token.kind == TOK_COLOR) {
                bits2 |= TOK_ECL;
            }
            bits1 |= TOK_ECL;
            break;
        case TOK_PID:
            next_token();
            expect(TOK_COLON);
            if (token.kind == TOK_NUMBER && token.size == 9) {
                bits2 |= TOK_PID;
            }
            bits1 |= TOK_PID;
            break;
        case TOK_CID:
        default:
            next_token();
            break;
        }
    }
    return res;
}

void init_map(void) {
    map_insert("byr", TOK_BYR);
    map_insert("iyr", TOK_IYR);
    map_insert("eyr", TOK_EYR);
    map_insert("hgt", TOK_HGT);
    map_insert("hcl", TOK_HCL);
    map_insert("ecl", TOK_ECL);
    map_insert("pid", TOK_PID);
    map_insert("cid", TOK_CID);

    map_insert("cm", TOK_CM);
    map_insert("in", TOK_IN);

    map_insert("amb", TOK_COLOR);
    map_insert("blu", TOK_COLOR);
    map_insert("brn", TOK_COLOR);
    map_insert("gry", TOK_COLOR);
    map_insert("grn", TOK_COLOR);
    map_insert("hzl", TOK_COLOR);
    map_insert("oth", TOK_COLOR);
}

int main(void) {
    char *tmp = aoc_read_input("input04.txt");
    buffer = tmp;

    init_map();

    Result res = solve();
    printf("%d\n%d\n", res.part1, res.part2);

    free(tmp);
}
