#include <ctype.h>
#include <stdbool.h>

#include "common.h"

typedef enum Token_kind {
    TOKEN_EOF,
    TOKEN_NUMBER,
    TOKEN_DASH,
    TOKEN_COLON,
    TOKEN_STRING,
    TOKEN_CHAR,
} Token_kind;

typedef struct Token {
    const char *start;
    const char *end;
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
    case '-':
        token.kind = TOKEN_DASH;
        buffer++;
        break;
    case ':':
        token.kind = TOKEN_COLON;
        buffer++;
        break;
    case 'a':
    case 'b':
    case 'c':
    case 'd':
    case 'e':
    case 'f':
    case 'g':
    case 'h':
    case 'i':
    case 'j':
    case 'k':
    case 'l':
    case 'm':
    case 'n':
    case 'o':
    case 'p':
    case 'q':
    case 'r':
    case 's':
    case 't':
    case 'u':
    case 'v':
    case 'w':
    case 'x':
    case 'y':
    case 'z':
        token.kind = TOKEN_STRING;
        while (islower(*buffer)) {
            buffer++;
        }
        if ((buffer - token.start) == 1) {
            token.kind = TOKEN_CHAR;
        }
        break;
    default:
        die("invalid token\n");
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

int parse(bool (*valid_password)(int, int, char)) {
    int num_valid = 0;

    next_token();
    while (token.kind != TOKEN_EOF) {
        if (token.kind != TOKEN_NUMBER) {
            die("expected number\n");
        }
        int i = atoi(token.start);
        next_token();

        expect(TOKEN_DASH);

        if (token.kind != TOKEN_NUMBER) {
            die("expected number\n");
        }
        int j = atoi(token.start);
        next_token();

        if (token.kind != TOKEN_CHAR) {
            die("expected char\n");
        }
        char c = *token.start;
        next_token();

        expect(TOKEN_COLON);

        if (token.kind != TOKEN_STRING) {
            die("expected string\n");
        }
        if (valid_password(i, j, c)) {
            num_valid++;
        }
        next_token();
    }
    return num_valid;
}

bool part1(int low, int high, char c) {
    int count = 0;
    for (const char *it = token.start; it != token.end; it++) {
        if (*it == c) {
            count++;
        }
    }
    return ((count >= low) && (count <= high));
}

bool part2(int i, int j, char c) {
    return (token.start[i - 1] == c) ^ (token.start[j - 1] == c);
}

void solution(const char *input) {
    buffer = input;
    printf("%d\n", parse(part1));
    buffer = input;
    printf("%d\n", parse(part2));
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s [FILE]\n", argv[0]);
        exit(1);
    }
    char *input = read_input(argv[1]);
    solution(input);
    free(input);
}
