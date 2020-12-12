#include <ctype.h>

#include "common.h"

typedef enum Token_kind {
    TOKEN_EOF,
} Token_kind;

typedef struct Token {
    const char *start;
    size_t len;
    Token_kind kind;
} Token;

const char *buffer;
Token token;

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
    default:
        break;
    }

    token.len = (size_t)(buffer - token.start);
}

void expect(Token_kind kind) {
    if (token.kind == kind) {
        next_token();
    } else {
        die("unexpected token\n");
    }
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s [FILE]\n", argv[0]);
        exit(1);
    }
    char *input = read_input(argv[1]);
    buffer = input;
    free(input);
}
