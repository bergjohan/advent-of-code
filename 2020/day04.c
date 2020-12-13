#include <ctype.h>

#include "common.h"

typedef enum Token_kind {
    TOKEN_UNKNOWN,
    TOKEN_BYR,
    TOKEN_IYR,
    TOKEN_EYR,
    TOKEN_HGT,
    TOKEN_HCL,
    TOKEN_ECL,
    TOKEN_PID,
    TOKEN_CM,
    TOKEN_IN,
    TOKEN_COLOR,
    TOKEN_EOF,
    TOKEN_NEXT_BLOCK,
    TOKEN_COLON,
    TOKEN_HASH,
    TOKEN_NUMBER,
} Token_kind;

typedef enum Field {
    FIELD_BYR = 0x00000001,
    FIELD_IYR = 0x00000010,
    FIELD_EYR = 0x00000100,
    FIELD_HGT = 0x00001000,
    FIELD_HCL = 0x00010000,
    FIELD_ECL = 0x00100000,
    FIELD_PID = 0x01000000,
} Field;

#define BITMASK                                                                \
    (FIELD_BYR | FIELD_IYR | FIELD_EYR | FIELD_HGT | FIELD_HCL | FIELD_ECL |   \
     FIELD_PID)

typedef struct Token {
    const char *start;
    const char *end;
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
        token.kind =
            (Token_kind)(intptr_t)map_find_range(&map, token.start, buffer);
        break;
    }

    token.end = buffer;
}

void expect(Token_kind kind) {
    if (token.kind == kind) {
        next_token();
    } else {
        die("unexpected token\n");
    }
}

void init_tokens(void) {
    map_insert(&map, "byr", (void *)TOKEN_BYR);
    map_insert(&map, "iyr", (void *)TOKEN_IYR);
    map_insert(&map, "eyr", (void *)TOKEN_EYR);
    map_insert(&map, "hgt", (void *)TOKEN_HGT);
    map_insert(&map, "hcl", (void *)TOKEN_HCL);
    map_insert(&map, "ecl", (void *)TOKEN_ECL);
    map_insert(&map, "pid", (void *)TOKEN_PID);

    map_insert(&map, "cm", (void *)TOKEN_CM);
    map_insert(&map, "in", (void *)TOKEN_IN);

    map_insert(&map, "amb", (void *)TOKEN_COLOR);
    map_insert(&map, "blu", (void *)TOKEN_COLOR);
    map_insert(&map, "brn", (void *)TOKEN_COLOR);
    map_insert(&map, "gry", (void *)TOKEN_COLOR);
    map_insert(&map, "grn", (void *)TOKEN_COLOR);
    map_insert(&map, "hzl", (void *)TOKEN_COLOR);
    map_insert(&map, "oth", (void *)TOKEN_COLOR);
}

void solution(void) {
    uint32_t bits1 = 0;
    uint32_t bits2 = 0;
    int num_valid1 = 0;
    int num_valid2 = 0;

    init_tokens();

    next_token();
    while (token.kind != TOKEN_EOF) {
        if (token.kind == TOKEN_NEXT_BLOCK) {
            if ((bits1 & BITMASK) == BITMASK) {
                num_valid1++;
            }
            if ((bits2 & BITMASK) == BITMASK) {
                num_valid2++;
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
                    bits2 |= FIELD_BYR;
                }
                next_token();
            }
            bits1 |= FIELD_BYR;
            break;
        case TOKEN_IYR:
            next_token();
            expect(TOKEN_COLON);
            if (token.kind == TOKEN_NUMBER) {
                int year = atoi(token.start);
                if (year >= 2010 && year <= 2020) {
                    bits2 |= FIELD_IYR;
                }
                next_token();
            }
            bits1 |= FIELD_IYR;
            break;
        case TOKEN_EYR:
            next_token();
            expect(TOKEN_COLON);
            if (token.kind == TOKEN_NUMBER) {
                int year = atoi(token.start);
                if (year >= 2020 && year <= 2030) {
                    bits2 |= FIELD_EYR;
                }
                next_token();
            }
            bits1 |= FIELD_EYR;
            break;
        case TOKEN_HGT:
            next_token();
            expect(TOKEN_COLON);
            if (token.kind == TOKEN_NUMBER) {
                int height = atoi(token.start);
                next_token();
                if (token.kind == TOKEN_CM) {
                    if (height >= 150 && height <= 193) {
                        bits2 |= FIELD_HGT;
                    }
                    next_token();
                } else if (token.kind == TOKEN_IN) {
                    if (height >= 59 && height <= 76) {
                        bits2 |= FIELD_HGT;
                    }
                    next_token();
                }
            }
            bits1 |= FIELD_HGT;
            break;
        case TOKEN_HCL:
            next_token();
            expect(TOKEN_COLON);
            if (token.kind == TOKEN_HASH &&
                (token.end - token.start - 1) == 6) {
                bits2 |= FIELD_HCL;
            }
            bits1 |= FIELD_HCL;
            break;
        case TOKEN_ECL:
            next_token();
            expect(TOKEN_COLON);
            if (token.kind == TOKEN_COLOR) {
                bits2 |= FIELD_ECL;
            }
            bits1 |= FIELD_ECL;
            break;
        case TOKEN_PID:
            next_token();
            expect(TOKEN_COLON);
            if (token.kind == TOKEN_NUMBER && (token.end - token.start) == 9) {
                bits2 |= FIELD_PID;
            }
            bits1 |= FIELD_PID;
            break;
        default:
            next_token();
            break;
        }
    }

    printf("%d\n", num_valid1);
    printf("%d\n", num_valid2);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s [FILE]\n", argv[0]);
        exit(1);
    }
    char *input = read_input(argv[1]);
    buffer = input;
    solution();
    free(input);
}
