#include <limits>
#include <sstream>
#include <vector>

#include "utils.h"

struct Result {
  int min_rolls;
  std::vector<int> inaccessible_squares;

  friend bool operator==(const Result &a, const Result &b) {
    return (a.min_rolls == b.min_rolls) &&
           (a.inaccessible_squares == b.inaccessible_squares);
  }
};

std::vector<Result> parse(std::istream &in) {
  std::vector<Result> results;
  while (!in.eof()) {
    Result ret;
    for (int i = 0; i < 4; i++) {
      std::string line;
      if (!std::getline(in, line)) {
        return results;
      }
      std::istringstream ss(line);
      switch (i) {
      case 0:
        ss >> ret.min_rolls;
        break;
      case 2: {
        int t;
        while (ss >> t) {
          ret.inaccessible_squares.push_back(t);
        }
      } break;
      }
    }
    results.push_back(ret);
  }
  return results;
}

int main(int argc, char **argv) {
  auto args = parse_console_args(argc, argv);
  std::ifstream f1(args.at("1").data());
  std::ifstream f2(args.at("2").data());

  auto r1 = parse(f1);
  auto r2 = parse(f2);
  bool all_match = true;

  for (unsigned int i = 0; i < std::min(r1.size(), r2.size()); i++) {
    if (r1[i] != r2[i]) {
      if (all_match) {
        all_match = false;
      }
      std::cout << "Testcase " << (i + 1) << " doesn't match. " << std::endl
                << "Minimum rolls: " << r1[i].min_rolls << " vs "
                << r2[i].min_rolls << std::endl
                << "inaccessible squares: " << r1[i].inaccessible_squares.size()
                << " vs " << r2[i].inaccessible_squares.size() << std::endl;
    }
  }

  if (r1.size() != r2.size()) {
    std::cout << "The number of testcases differ" << std::endl;
  } else {
    if (all_match) {
      std::cout << "Outputs are compatible" << std::endl;
    } else {
      std::cout << "Outputs are not compatible" << std::endl;
    }
  }
}
