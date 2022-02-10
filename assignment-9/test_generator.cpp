#include <fstream>
#include <iostream>
#include <random>
#include <unordered_map>

#include "1.h"
#include "2.h"

std::random_device dev;
std::mt19937 eng(dev());

std::unordered_map<std::string_view, std::string_view>
parse_console_args(int argc, char **argv) {
  std::unordered_map<std::string_view, std::string_view> args;
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      if (i + 1 < argc) {
        if (argv[i + 1][0] == '-') {
          args.emplace(argv[i] + 1 + (argv[i][1] == '-'), "true");
        } else {
          args.emplace(argv[i] + 1 + (argv[i][1] == '-'), argv[i + 1]);
        }
      } else {
        args.emplace(argv[i] + 1 + (argv[i][1] == '-'), "true");
      }
    }
  }
  return args;
}

void generate1() {
  std::uniform_int_distribution<int> N(1, 50), M(1, 50);
  std::uniform_int_distribution<char> C('A', 'Z');
  bool empty;
  std::string x, y, lcs;
  do {
    int n = N(eng);
    int m = M(eng);
    x.clear();
    y.clear();
    for (int i = 0; i < n; i++) {
      x.push_back(C(eng));
    }
    for (int i = 0; i < m; i++) {
      y.push_back(C(eng));
    }
    lcs = find_lcs(x, y);
  } while (lcs.empty());

  std::cout << x << std::endl << y << std::endl;
  std::cout << "Expected answer:" << std::endl;
  std::cout << lcs.length() << std::endl;
  std::cout << lcs << std::endl;
}

void generate2() {
  std::uniform_int_distribution<int> N(1, 10), A(1, 10000);
  int n = N(eng);
  std::vector<std::vector<int>> a(n, std::vector<int>(n));

  std::cout << n << std::endl;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      std::cout << (a[i][j] = A(eng)) << ' ';
    }
    std::cout << std::endl;
  }

  std::cout << "Expected answer:" << std::endl;
  std::cout << find_min_travel_cost(n, a) << std::endl;
}

int main(int argc, char **argv) {
  auto args = parse_console_args(argc, argv);
  if (args["1"] == "true") {
    std::cout << "Test case for 1:" << std::endl;
    generate1();
  }
  if (args["2"] == "true") {
    std::cout << "Test case for 2:" << std::endl;
    generate2();
  }
}
