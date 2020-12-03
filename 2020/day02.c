#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>

#include "aoc_common.h"

typedef enum Token_kind {
    TOK_EOF,
    TOK_NUMBER,
    TOK_DASH,
    TOK_COLON,
    TOK_CHAR,
    TOK_STRING
} Token_kind;

typedef struct String {
    const char *text;
    size_t size;
} String;

typedef struct Token {
    Token_kind kind;
    union {
        String str;
        char c;
        int num;
    };
} Token;

const char *buffer;
Token token;

void next_token(void) {
    while (isspace(*buffer)) {
        buffer++;
    }

    switch (*buffer) {
    case 0:
        token.kind = TOK_EOF;
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
        token.kind = TOK_NUMBER;
        token.num = atoi(buffer);
        while (isdigit(*buffer)) {
            buffer++;
        }
        break;
    case '-':
        token.kind = TOK_DASH;
        buffer++;
        break;
    case ':':
        token.kind = TOK_COLON;
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
        token.kind = TOK_STRING;
        token.str.text = buffer;
        while (islower(*buffer)) {
            buffer++;
        }
        token.str.size = (size_t)(buffer - token.str.text);
        if (token.str.size == 1) {
            token.c = *token.str.text;
            token.kind = TOK_CHAR;
        }
        break;
    default:
        aoc_die("invalid token\n");
    }
}

void expect(Token_kind kind) {
    if (token.kind == kind) {
        next_token();
    } else {
        aoc_die("unexpected token\n");
    }
}

int parse(bool (*valid_password)(int, int, char)) {
    int i;
    int j;
    char c;
    int num_valid = 0;

    next_token();
    while (token.kind != TOK_EOF) {
        if (token.kind != TOK_NUMBER) {
            aoc_die("expected number\n");
        }
        i = token.num;
        next_token();

        expect(TOK_DASH);

        if (token.kind != TOK_NUMBER) {
            aoc_die("expected number\n");
        }
        j = token.num;
        next_token();

        if (token.kind != TOK_CHAR) {
            aoc_die("expected char\n");
        }
        c = token.c;
        next_token();

        expect(TOK_COLON);

        if (token.kind != TOK_STRING) {
            aoc_die("expected string\n");
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
    for (size_t i = 0; i < token.str.size; i++) {
        if (token.str.text[i] == c) {
            count++;
        }
    }
    return ((count >= low) && (count <= high));
}

bool part2(int i, int j, char c) {
    return (token.str.text[i - 1] == c) ^ (token.str.text[j - 1] == c);
}

int main(void) {
    char *tmp = aoc_read_input("input02.txt");
    buffer = tmp;
    printf("%d\n", parse(part1));
    buffer = tmp;
    printf("%d\n", parse(part2));
    free(tmp);
}
