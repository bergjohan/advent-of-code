#include "common.h"

int part2(const char *input) {
    uint32_t answers = 0;
    uint32_t res = UINT32_MAX;
    int sum = 0;

    while (*input) {
        if (*input == '\n') {
            res &= answers;
            answers = 0;
            input++;
            if (*input == '\n') {
                sum += __builtin_popcount(res);
                res = UINT32_MAX;
                input++;
            }
        } else {
            answers |= 1 << (*input - 'a');
            input++;
        }
    }
    sum += __builtin_popcount(res);
    return sum;
}

int part1(const char *input) {
    uint32_t answers = 0;
    int sum = 0;

    while (*input) {
        if (*input == '\n') {
            input++;
            if (*input == '\n') {
                sum += __builtin_popcount(answers);
                answers = 0;
                input++;
            }
        } else {
            answers |= 1 << (*input - 'a');
            input++;
        }
    }
    sum += __builtin_popcount(answers);
    return sum;
}

void solution(const char *input) {
    printf("%d\n", part1(input));
    printf("%d\n", part2(input));
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
