#include <cassert>
#include <numeric>
#include <vector>

static int fuel_required(int mass) { return mass / 3 - 2; }

static int part1(const std::vector<int> &input) {
  return std::accumulate(input.begin(), input.end(), 0, [](int acc, int mass) {
    return acc + fuel_required(mass);
  });
}

static int part2(const std::vector<int> &input) {
  return std::accumulate(input.begin(), input.end(), 0, [](int acc, int mass) {
    for (int fuel = fuel_required(mass); fuel > 0; fuel = fuel_required(fuel)) {
      acc += fuel;
    }
    return acc;
  });
}

int main() {
  const std::vector<int> input = {
      138828, 82053,  75644,  86659,  85337,  63842,  88120,  144319, 58294,
      82233,  81964,  108059, 117326, 65553,  105367, 122086, 70431,  89418,
      108818, 61254,  97351,  75645,  88868,  140241, 112242, 119866, 86519,
      81313,  60462,  89313,  134057, 96984,  57528,  112293, 94987,  71785,
      104896, 108760, 68391,  95901,  112259, 51337,  75020,  99526,  132617,
      63797,  121541, 111211, 70179,  51681,  60569,  56247,  55871,  61344,
      56965,  76208,  137230, 99499,  109960, 81224,  93064,  137496, 111009,
      99572,  135425, 121418, 79337,  110813, 87081,  110898, 71344,  99419,
      139493, 52847,  112560, 144685, 79700,  139438, 127821, 133377, 82892,
      107311, 118376, 96197,  66839,  63551,  145689, 94461,  93739,  106350,
      67884,  124828, 141116, 78967,  143686, 137159, 85746,  115543, 73157,
      132343};

  assert(part1(input) == 3226407);
  assert(part2(input) == 4836738);
}
