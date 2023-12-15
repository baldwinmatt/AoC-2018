#include "aoc/helpers.h"

#include <map>

namespace {
  using Result = std::pair<int, std::string>;
  using MappedFileSource = aoc::MappedFileSource<char>;

  constexpr std::string_view SampleInput(R"(abcdef
bababc
abbcde
abcccd
aabcdd
abcdee
ababab)");
  constexpr int SR_Part1 = 12;
  const std::string SR_Part2{"abcde"};

  const auto CheckSum =[](std::string_view s) {
    std::map<char, uint_fast8_t> chars;
    for (char c : s) {
      auto it = chars.emplace(c, 0);
      it.first->second++;
    }

    std::pair<int32_t, int32_t> two_three{0, 0};
    for (const auto& p : chars) {
      if (p.second == 2) {
        two_three.first++;
      } else if (p.second == 3) {
        two_three.second++;
      }
    }
    return two_three;
  };

  const auto DiffStrings = [](std::string_view a, std::string_view b) {
    int diff = 0;
    for (size_t i = 0; i < a.size(); i++) {
      diff += (a[i] != b[i]);
    }
    return diff;
  };
  const auto GetStringUnion = [](std::string_view a, std::string_view b) {
    std::string r;
    for (size_t i = 0; i < a.size(); i++) {
      if (a[i] == b[i]) {
        r.push_back(a[i]);
      }
    }
    return r;
  };

  const auto LoadInput = [](auto f) {
    Result r{0, {} };
    std::string_view line;
    std::pair<int32_t, int32_t> two_three{0, 0};
    std::vector<std::string_view> strings;
    while (aoc::getline(f, line)) {
      auto t = CheckSum(line);
      two_three.first += (t.first > 0);
      two_three.second += (t.second > 0);
      strings.push_back(line);
    }
    DEBUG_PRINT(two_three.first << " * " << two_three.second);
    r.first = two_three.first * two_three.second;

    for (size_t i = 0; r.second.empty() && i < strings.size() - 1; i++) {
      for (size_t j = i + 1; r.second.empty() && j < strings.size(); j++) {
        if (DiffStrings(strings[i], strings[j]) == 1) {
          r.second = GetStringUnion(strings[i], strings[j]);
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

  aoc::print_results(r.first, r.second);

  if (inTest) {
    aoc::assert_result(r.first, SR_Part1);
    aoc::assert_result(r.second, SR_Part2);
  }

  return 0;
}
