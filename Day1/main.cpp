#include "aoc/helpers.h"
#include <unordered_set>

namespace {
  using Result = std::pair<int64_t, int64_t>;
  using MappedFileSource = aoc::MappedFileSource<char>;

  constexpr std::string_view SampleInput(R"(+1
-2
+3
+1)");
  constexpr int64_t SR_Part1 = 3;
  constexpr int64_t SR_Part2 = 2;

  const auto LoadInput = [](auto f) {
    Result r{0, INT64_MIN};
    std::string_view line;
    std::vector<int64_t> v;
    std::unordered_set<int64_t> seen;
    while (aoc::getline(f, line)) {
      v.push_back(aoc::stoi(line));
      r.first += v.back();

      if (!seen.insert(r.first).second) {
        r.second = r.first;
      }
    }

    auto freq = r.first;
    while (r.second == INT64_MIN) {
      for (auto i : v) {
        freq += i;
        if (!seen.insert(freq).second) {
          r.second = freq;
          break;
        }
      }
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
