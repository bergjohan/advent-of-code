#include <cassert>
#include <vector>

enum class Opcode { add = 1, mul = 2, halt = 99 };

static int run(std::vector<int> memory, int noun, int verb) {
  memory[1] = noun;
  memory[2] = verb;

  for (std::size_t i = 0; i < memory.size(); i += 4) {
    int addr1 = memory[i + 1];
    int addr2 = memory[i + 2];
    int addr3 = memory[i + 3];

    switch (static_cast<Opcode>(memory[i])) {
      case Opcode::add:
        memory[addr3] = memory[addr1] + memory[addr2];
        break;
      case Opcode::mul:
        memory[addr3] = memory[addr1] * memory[addr2];
        break;
      case Opcode::halt:
        return memory[0];
      default:
        return -1;
    }
  }
  return -1;
}

static int part1(const std::vector<int> &input) { return run(input, 12, 2); }

static int part2(const std::vector<int> &input) {
  for (int noun = 0; noun < 100; ++noun) {
    for (int verb = 0; verb < 100; ++verb) {
      if (run(input, noun, verb) == 19690720) {
        return 100 * noun + verb;
      }
    }
  }
  return -1;
}

int main() {
  const std::vector<int> input = {
      1, 0,   0,  3,  1,  1,  2,  3,   1, 3,  4,   3,   1, 5,   0,  3,
      2, 10,  1,  19, 2,  9,  19, 23,  2, 13, 23,  27,  1, 6,   27, 31,
      2, 6,   31, 35, 2,  13, 35, 39,  1, 39, 10,  43,  2, 43,  13, 47,
      1, 9,   47, 51, 1,  51, 13, 55,  1, 55, 13,  59,  2, 59,  13, 63,
      1, 63,  6,  67, 2,  6,  67, 71,  1, 5,  71,  75,  2, 6,   75, 79,
      1, 5,   79, 83, 2,  83, 6,  87,  1, 5,  87,  91,  1, 6,   91, 95,
      2, 95,  6,  99, 1,  5,  99, 103, 1, 6,  103, 107, 1, 107, 2,  111,
      1, 111, 5,  0,  99, 2,  14, 0,   0};

  assert(part1(input) == 2890696);
  assert(part2(input) == 8226);
}
