#include <ctype.h>

#include "common.h"

typedef enum Token_kind {
    TOKEN_UNKNOWN,
    TOKEN_BYR = 0x00000001,
    TOKEN_IYR = 0x00000010,
    TOKEN_EYR = 0x00000100,
    TOKEN_HGT = 0x00001000,
    TOKEN_HCL = 0x00010000,
    TOKEN_ECL = 0x00100000,
    TOKEN_PID = 0x01000000,
    TOKEN_CM,
    TOKEN_IN,
    TOKEN_COLOR,
    TOKEN_EOF,
    TOKEN_NEXT_BLOCK,
    TOKEN_COLON,
    TOKEN_HASH,
    TOKEN_NUMBER,
} Token_kind;

#define BITMASK                                                                \
    (TOKEN_BYR | TOKEN_IYR | TOKEN_EYR | TOKEN_HGT | TOKEN_HCL | TOKEN_ECL |   \
     TOKEN_PID)

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
        buffer++;
        if (*buffer == '\n') {
            token.kind = TOKEN_NEXT_BLOCK;
            break;
        }
        while (isspace(*buffer)) {
            buffer++;
        }
        goto next;
    case ':':
        token.kind = TOKEN_COLON;
        buffer++;
        break;
    case '#':
        token.kind = TOKEN_HASH;
        buffer++;
        while (isxdigit(*buffer)) {
            buffer++;
        }
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
        token.kind = (Token_kind)(intptr_t)map_find(
            &map, token.start, (size_t)(buffer - token.start));
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

typedef struct Result {
    int part1;
    int part2;
} Result;

Result solution(void) {
    Result res = {0};
    uint32_t bits1 = 0;
    uint32_t bits2 = 0;

    next_token();
    while (token.kind != TOKEN_EOF) {
        if (token.kind == TOKEN_NEXT_BLOCK) {
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
        case TOKEN_BYR:
            next_token();
            expect(TOKEN_COLON);
            if (token.kind == TOKEN_NUMBER) {
                int year = atoi(token.start);
                if (year >= 1920 && year <= 2002) {
                    bits2 |= TOKEN_BYR;
                }
                next_token();
            }
            bits1 |= TOKEN_BYR;
            break;
        case TOKEN_IYR:
            next_token();
            expect(TOKEN_COLON);
            if (token.kind == TOKEN_NUMBER) {
                int year = atoi(token.start);
                if (year >= 2010 && year <= 2020) {
                    bits2 |= TOKEN_IYR;
                }
                next_token();
            }
            bits1 |= TOKEN_IYR;
            break;
        case TOKEN_EYR:
            next_token();
            expect(TOKEN_COLON);
            if (token.kind == TOKEN_NUMBER) {
                int year = atoi(token.start);
                if (year >= 2020 && year <= 2030) {
                    bits2 |= TOKEN_EYR;
                }
                next_token();
            }
            bits1 |= TOKEN_EYR;
            break;
        case TOKEN_HGT:
            next_token();
            expect(TOKEN_COLON);
            if (token.kind == TOKEN_NUMBER) {
                int height = atoi(token.start);
                next_token();
                if (token.kind == TOKEN_CM) {
                    if (height >= 150 && height <= 193) {
                        bits2 |= TOKEN_HGT;
                    }
                    next_token();
                } else if (token.kind == TOKEN_IN) {
                    if (height >= 59 && height <= 76) {
                        bits2 |= TOKEN_HGT;
                    }
                    next_token();
                }
            }
            bits1 |= TOKEN_HGT;
            break;
        case TOKEN_HCL:
            next_token();
            expect(TOKEN_COLON);
            if (token.kind == TOKEN_HASH && (token.size - 1) == 6) {
                bits2 |= TOKEN_HCL;
            }
            bits1 |= TOKEN_HCL;
            break;
        case TOKEN_ECL:
            next_token();
            expect(TOKEN_COLON);
            if (token.kind == TOKEN_COLOR) {
                bits2 |= TOKEN_ECL;
            }
            bits1 |= TOKEN_ECL;
            break;
        case TOKEN_PID:
            next_token();
            expect(TOKEN_COLON);
            if (token.kind == TOKEN_NUMBER && token.size == 9) {
                bits2 |= TOKEN_PID;
            }
            bits1 |= TOKEN_PID;
            break;
        default:
            next_token();
            break;
        }
    }
    return res;
}

void init_map(void) {
    map_insert(&map, "byr", 3, (void *)TOKEN_BYR);
    map_insert(&map, "iyr", 3, (void *)TOKEN_IYR);
    map_insert(&map, "eyr", 3, (void *)TOKEN_EYR);
    map_insert(&map, "hgt", 3, (void *)TOKEN_HGT);
    map_insert(&map, "hcl", 3, (void *)TOKEN_HCL);
    map_insert(&map, "ecl", 3, (void *)TOKEN_ECL);
    map_insert(&map, "pid", 3, (void *)TOKEN_PID);

    map_insert(&map, "cm", 2, (void *)TOKEN_CM);
    map_insert(&map, "in", 2, (void *)TOKEN_IN);

    map_insert(&map, "amb", 3, (void *)TOKEN_COLOR);
    map_insert(&map, "blu", 3, (void *)TOKEN_COLOR);
    map_insert(&map, "brn", 3, (void *)TOKEN_COLOR);
    map_insert(&map, "gry", 3, (void *)TOKEN_COLOR);
    map_insert(&map, "grn", 3, (void *)TOKEN_COLOR);
    map_insert(&map, "hzl", 3, (void *)TOKEN_COLOR);
    map_insert(&map, "oth", 3, (void *)TOKEN_COLOR);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s [FILE]\n", argv[0]);
        exit(1);
    }
    char *input = read_input(argv[1]);
    buffer = input;
    init_map();
    Result res = solution();
    printf("%d\n%d\n", res.part1, res.part2);
    free(input);
}
