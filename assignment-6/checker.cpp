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

  if (parse(f1) == parse(f2)) {
    std::cout << "Outputs are compatible" << std::endl;
  } else {
    std::cout << "Outputs are not compatible" << std::endl;
  }
}
