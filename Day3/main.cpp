#include "aoc/helpers.h"
#include "aoc/point.h"

namespace {
  using Result = std::pair<int, int>;
  using MappedFileSource = aoc::MappedFileSource<char>;

  constexpr std::string_view SampleInput(R"(#1 @ 1,3: 4x4
#2 @ 3,1: 4x4
#3 @ 5,5: 2x2)");
  constexpr int SR_Part1 = 4;
  constexpr int SR_Part2 = 3;

  using Row = std::array<int, 1000>;
  using Grid = std::array<Row, 1000>;

  const auto LoadInput = [](auto f) {
    Result r{0, 0};
    std::string_view line;
    Grid g{0};
    std::vector<std::pair<aoc::point, aoc::point>> claims;
    while (aoc::getline(f, line)) {
      auto parts = aoc::split(line, " ");

      auto offset = parts[2];
      auto dims = parts[3];

      offset.remove_suffix(1);
      claims.emplace_back(aoc::point::from_parts(aoc::split(offset, ",")),
                          aoc::point::from_parts(aoc::split(dims, "x")));
      const auto &claim = claims.back();
      auto &p = claim.first;
      auto &d = claim.second;

      for (auto x = p.x; x < p.x + d.x; ++x) {
        for (auto y = p.y; y < p.y + d.y; ++y) {
          ++g[x][y];
        }
      }
    }

    for (const auto &row : g) {
      for (const auto &cell : row) {
        if (cell > 1) {
          ++r.first;
        }
      }
    }

    int64_t id = 1;
    for (const auto &claim : claims) {
      auto &p = claim.first;
      auto &d = claim.second;
      bool intact = true;
      for (auto x = p.x; intact && x < p.x + d.x; ++x) {
        for (auto y = p.y; intact && y < p.y + d.y; ++y) {
          intact = g[x][y] == 1;
        }
      }

      if (intact) {
        r.second = id;
        break;
      }
      id++;
    }
    return r;
  };
}

int main(int argc, char** argv) {
  aoc::AutoTimer t;
  const bool inTest = argc < 2;

  Result r;
  if (inTest) {
    r = LoadInput(SampleInput);
  } else {
    std::unique_ptr<MappedFileSource>m(new MappedFileSource(argc, argv));
    std::string_view f(m->data(), m->size());
    r = LoadInput(f);
  }

  int part1 = 0;
  int part2 = 0;

  std::tie(part1, part2) = r;

  aoc::print_results(part1, part2);

  if (inTest) {
    aoc::assert_result(part1, SR_Part1);
    aoc::assert_result(part2, SR_Part2);
  }

  return 0;
}
