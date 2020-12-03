#include <inttypes.h>
#include <stdio.h>
#include <string.h>

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
#define LINE_LEN 31

uint64_t check_slope(const char *input[], size_t size, size_t right,
                     size_t down) {
    uint64_t num_trees = 0;
    size_t j = 0;

    for (size_t i = down; i < size; i += down) {
        const char *p = input[i];
        j = (j + right) % LINE_LEN;
        if (p[j] == '#') {
            num_trees++;
        }
    }
    return num_trees;
}

uint64_t part1(const char *input[], size_t size) {
    return check_slope(input, size, 3, 1);
}

uint64_t part2(const char *input[], size_t size) {
    return check_slope(input, size, 1, 1) *
           check_slope(input, size, 3, 1) *
           check_slope(input, size, 5, 1) *
           check_slope(input, size, 7, 1) *
           check_slope(input, size, 1, 2);
}

int main(void) {
    const char *input[] = {
        ".........#..##.##..............",
        "#...#.#..#.....................",
        ".#...#..#...................#..",
        "........##..#...#..............",
        ".#.#.....#..#..##......#.......",
        "....#..#...#..##........#..##..",
        "...#....##........#.......#.#..",
        "....#................#...###..#",
        "...#...#.#..#....#.......####.#",
        ".....#...#..........#...#..#.#.",
        "....#..#............#.#.#.#..#.",
        "..#....#.###..#............#...",
        ".....#.............#.#.........",
        ".#.##............##.........#..",
        "...##...#..#....#.##..#.....#..",
        "..............#.#.........#.##.",
        "...........#.....##....##......",
        ".......#............#...#......",
        "............#.#....#....#..#..#",
        "....#................####......",
        "...#.........................##",
        "..........#........#.#.........",
        "....#.#....#...........#......#",
        "..#.#..##......##..##..#..#.#..",
        "...#.....##......#.#.#.........",
        ".........#.#....#...#.#.#......",
        ".......#.......###.#.......#...",
        "..#............##..#.......#...",
        "...#....#......#...#...#...#...",
        "......#..#.#.....##......#.....",
        "...........##...##...#....#.##.",
        "#.##..#.....##..#.#............",
        ".#.#.....##......#.##........#.",
        "..#...#...#...#..........#...##",
        "...##.........................#",
        ".....#......#.....##....#.....#",
        "..#........#...................",
        "#......#..#.#..#..#.#..#...#...",
        "...............#..........#....",
        ".....#...........#......#....#.",
        "........#..#...............#...",
        ".........#...#.......#.#..#...#",
        "..#..#......#.##..........#....",
        ".#...#....#.....#.............#",
        ".##.....#.........#......#..#..",
        "........#..##.......#......#...",
        ".......#.....###......#..#.....",
        ".......#.#.......#.............",
        "...#................##.#.......",
        "..##..#...#.#...#.#..#.#.#.##..",
        ".......#.#............#...#....",
        "#...#.....#......#..........##.",
        ".#.......#......#.......#.#.#..",
        ".#.##.#.#...........#..........",
        ".......#.....#....#.#.##......#",
        ".###..#...#.............##.....",
        "......#......#.................",
        "##...#.#...##...#.#.....#....#.",
        "#.............#....##...#....#.",
        "#.#...#....#........#.###..##..",
        "......#.........#......#.#.#.#.",
        "..#.#.#.....#........#..#...#..",
        "#.##....#.#......#...........#.",
        ".#.#.####.........#..#.##....##",
        "......##...............#......#",
        ".......#.........#........#.#..",
        "....#....#..#.##.........#..#..",
        ".#..........#.##....#.#..#.....",
        "#..#.#............#..#....#.#.#",
        "..................#..#.........",
        "##..##.#....#.................#",
        "..................#........#..#",
        ".....#.........#.......##......",
        ".....................#.#..#...#",
        ".....#.........#..........#.#..",
        "...#.#..#..#.#.#.......#.......",
        ".....#.....#.#.........#.....##",
        ".............##....#....##.#...",
        ".#......#........##..#...###...",
        "........#.......##.##.#......#.",
        "..#....................#.##..#.",
        "......#.......#..#....##.#.....",
        "...#....#.......##...#.......#.",
        ".#.#..#.#..........##..........",
        "....#.......##...........#.....",
        "###....#.......#..#...#.....##.",
        "...#......#.........#..#.#..#.#",
        "#.........#..##.#..............",
        ".#.....#..##.#..#..###.....##.#",
        "..........#..#....##.......#...",
        ".#..#.#...#...##.#..#.##.#.....",
        "#....#...#........#......##....",
        "..#.####....#.#........#....#..",
        "#......#............#.#........",
        "...#..#.......##...........#...",
        ".........#..#.#..#.###.#...#..#",
        "..#....##.......#.............#",
        "............#..#......#........",
        "........#......#..............#",
        "..#.#.#...........#............",
        "...........#......##.#.#.......",
        ".#..........#...........#..#...",
        ".....#...#..#.........##...#...",
        ".......#....##....#.#.........#",
        "..#.#......#.......#...##.#....",
        ".....#..........#........#.....",
        "#.......#.......#............#.",
        "..##.#.....#.##.#.#.#..#.......",
        "..#...#.......#.###............",
        ".#...#..#....#...#...#..#....##",
        ".....#.....#...................",
        ".......................#......#",
        "......#...##.........#...#..#..",
        ".....#..#.....#..............#.",
        ".#.##..#..#....................",
        "....#..#...#....#.............#",
        "..###..#...#......#.....#......",
        "..#......###....#.....#.....###",
        "...#.##..#...#.....#........#..",
        ".#.#...........##....#...#.##..",
        ".......#...##......#..#..#.....",
        "#.............#..#...##.#..#..#",
        "..........#......#.......#.....",
        "...............#.#.#....#...#..",
        "#.......#.#..#.....#........#..",
        ".#.#.#.......#..#.........##...",
        "......#.....#.#....#...........",
        "..#.....##.#........##.#......#",
        "...###...#..#.........#........",
        "....#...................#..#...",
        ".##........#...................",
        "....#..#...........#.#.........",
        ".....#.......#...#....#.#......",
        ".........#...#.......#.#...#...",
        ".......#.#..#....#....#.......#",
        "..#.............#..............",
        ".#...#..#.#.#..#............#..",
        "...#.##.##..#..#...........##..",
        "...........#...#..#.#........#.",
        "....#...#.....#...#.#....#...#.",
        ".......#.#...##..#.............",
        ".......................#....#..",
        "..#..#.....#...........#....#..",
        ".#..#...#.##........##....#....",
        "#.....##.#.#.......#.....#...#.",
        ".#....#.......................#",
        "#..##..###...#.........#.......",
        "..##...#...#..........#....#...",
        "......#..##......##.#.........#",
        "................#........#..#..",
        ".....#.#..#.....#.......#..#...",
        "..#..#.....#.......#..#..#...#.",
        ".#....#...#...#......##.....#..",
        "....#........#...#......##....#",
        "..#..........##......#......#..",
        "#.#.....#.....#.......#........",
        "...#...#......#....##.#..#...##",
        "...#....#...#.#...........##...",
        "#....##...#...#....#...........",
        "...#.#..#...#..............##..",
        "#..#..........##.#.#.....#.....",
        "..#...#.........#.#..........#.",
        "....#.....#..........#.........",
        "........................#......",
        ".#.....#.#...###...#....#......",
        "....##....#....#..#.##........#",
        "..#........#.........#.......#.",
        ".....#.#......#...#...#........",
        "........#..#.....#....###....#.",
        "...........#..#.#....#.#....##.",
        ".......#.....##.#............#.",
        "...............#........##.##..",
        ".............#...##......#...#.",
        "#...##.#.......#......###.....#",
        "..........#...#........#..#....",
        "....#....................#...#.",
        ".#......#...#.......#....#.#...",
        "....#.......................#..",
        "#...#...#...#.##....##.........",
        "..........#.#...##.#...#.......",
        "..#...............#....#..#...#",
        "#..#..#...#..#.........#...#...",
        ".....#..#..........#.##.#..##..",
        "........#......##.....#........",
        ".#....#.#.........#...#..#.#...",
        "....#..............##..........",
        "#...............#..............",
        "..###.........#....##.........#",
        ".........#.#....##........#...#",
        "....#.#..#......#...#..........",
        "...#.#.....#....#..#....#.#..#.",
        "............#..#.....#...##....",
        "...........#....#.#.#...#......",
        "#...............#....###.......",
        ".........#.....##.#..#..#......",
        "...#...##...###...............#",
        ".#......#.#.#.................#",
        ".........##..#............#....",
        "..#..#.....#.....#.#...........",
        ".#......##............#.#....#.",
        ".#.##..##.##..#.........#.....#",
        "...##.##......##.##......#.....",
        "##.....#.#...#...#...#..#......",
        "....................#......#...",
        ".....#.................#...###.",
        "...........#..#.........#.#....",
        "...#........#..#........#....#.",
        "#................#......###...#",
        ".............##.#.....#.#......",
        "...#...#.##..#........##.......",
        "#..#.##...#....#.#.............",
        ".#.........#.#..#...........#..",
        "....#...#.....#.#..........#...",
        ".#.#....###.......##.....#.##..",
        ".##....##......#......#.#....#.",
        "..#...#.#........#...#..##.....",
        "..............###..........##..",
        "#....#..##.....#.....#.....#...",
        "...#...#....................#..",
        ".#....#.#.....#.#..#..##.......",
        "#...##..###......#.............",
        "..........#.#....##.#........##",
        "..#..#.....#...#....#.#.#......",
        "#.....#........#..##.#.........",
        "....#.....#..........##......#.",
        "......#..#.....#........#.....#",
        ".....#..#....#...........#.##..",
        ".#....................#....#..#",
        "........#..#...........#.......",
        "#....#.#.......#........#.#..#.",
        "........#.....#...#............",
        "..#........#........#....#...#.",
        ".....##.......#..#..........#..",
        "......#.#......###...#....##..#",
        ".#..#.............#.#..........",
        "#.....##.#.#.#.#.#.....#.....#.",
        ".#..#.....#.......#.#.....#....",
        "###....##...#.#...#..#......###",
        ".#................#.....#.##...",
        "....##....#.#........###.#.#...",
        "#.#....#........#.....#.......#",
        "..........#..........#.##...#..",
        "....#....#..##......#..#.......",
        ".....#..........#.##...........",
        "##......#.#......#.##..........",
        "##..........##.......##........",
        "..#.....#....#.##..#..#..#.....",
        "......###...#...........#...###",
        "#..#.............##............",
        "...#.###.....#..#.........#.#..",
        "......#...............#...#.#..",
        ".....#...##.#...#.....#.#..#...",
        "..#..#.#....#.#................",
        "...............##.....#........",
        "......#.#.....#...#.........#..",
        "........#..#...#.#...#......#..",
        "#...........#.......#...##...#.",
        "........#.#...#..##..#.#...#...",
        "..#....#...#......#..........##",
        "..#..............##...##.#.....",
        "...#....#..#....##.........#.#.",
        ".#.#....#..........#.......#...",
        "...##....#.#....#....#.#...#...",
        "..............#..##........#..#",
        "..........#.#...##......#..#.#.",
        "#...##..#......................",
        ".......#........##.#.#.#.......",
        ".........##..#.#.......####....",
        "..#.............#..#........##.",
        "##..#..#...#....#.....#...#..#.",
        "..#.#...#.#.....#..............",
        "..#....#....#..##...#.#........",
        "##.....#..#...#................",
        "#....#.....................#...",
        "..............###.....#.#.#....",
        "..#......##.#....#.#...##......",
        "#...#.#......#...#.#......#....",
        "....#...................##.#...",
        ".........##......#.....#.####..",
        "##..#........#.....#......##..#",
        "...#..#...#...#.............#..",
        "#..#..#.#......###...#.........",
        ".......#.#..#........#....##..#",
        "............#..##.....#.#.#....",
        "#..#.....#.....#....##........#",
        "......#..........##............",
        ".....#...#...........#.........",
        "...........#....#...#....#.#...",
        "....#.........##.##.......#....",
        "......#....#...........#.##...#",
        ".##.#.#..##...#.....##.#...#...",
        ".......#.#....#...#...#....#...",
        ".#...##.#.#.....#..#....#......",
        ".#....###..#.......#......#...#",
        "..#.#.........#.........#.....#",
        ".......#.#.##..#.#.......##..#.",
        ".##............#.........#....#",
        ".#...##.###..#........##.#..#..",
        "..#........#.#.....##..##.#....",
        "...........#...........#.....#.",
        ".#...######..##...#.....#......",
        ".#.##.#.......#......#......#..",
        ".#.....#.....#........#........",
        "...#..#...#.##...#...........#.",
        ".......#.....#.......#.........",
        "............#...###...........#",
        "...#.......#.......##....#..#..",
        "##.......#....#....####........",
        ".......#.#......#..........#..#",
        "#.....##..#..#.....#....#...#..",
        "#............#........##.......",
        ".#.#...#.............#..##.....",
        ".#....#..#.#......#.##.......##",
        "...................##...##..###",
        "..#.....#...#................#.",
        "..#...#....#...#.#.#...#.....#.",
        ".....#............#....#...#..#",
        ".#.....#....#..#......#.#.....#",
        "............#.#.....####.##....",
        "....#......###....#...#....#...",
        "#.....#..#.....#..#...#.......#",
        "..#.#...#....#....##..#...##...",
        ".##..#..#..##....##...#........"
    };
    printf("%lu\n", part1(input, ARRAY_SIZE(input)));
    printf("%lu\n", part2(input, ARRAY_SIZE(input)));
}
