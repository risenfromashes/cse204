#include <cassert>
#include <cinttypes>
#include <fstream>
#include <iostream>
#include <random>
#include <unordered_set>
#include <vector>

#include "utils.h"

int main(int argc, char **argv) {
  auto args = parse_console_args(argc, argv);
  configure_io(args);
  bool verbose = args["verbose"] == "true";
  bool no_cascade = args["no_cascade"] == "true";

  std::random_device dev;
  std::mt19937 eng(dev());
  std::uniform_int_distribution<int> N(2, 8), X(2, 10000);

  int T = 1000;
  std::cout << T << std::endl;
  for (int t = 1; t <= T; t++) {
    int n = 2 * N(eng);
    int x = X(eng);
    int l = X(eng) % (x / 2);
    int s = X(eng) % (x / 2);

    if (no_cascade) {
      l /= 2;
      s /= 2;
    }

    std::cout << n << ' ' << x << std::endl;
    if (verbose) {
      std::cerr << t << ". " << n << ' ' << x << std::endl;
    }

    std::cout << l << std::endl;
    std::vector<int> jumps(x + 1, 0);

    for (int i = 0; i < l; i++) {
      int from, to;

      if (no_cascade) {
        do {
          from = std::clamp(X(eng), 2, x - 1);
          to = std::clamp(X(eng) % (x - from) + from + 1, from + 1, x);
        } while (jumps[to] || jumps[from]);
        jumps[to] = from;
      } else {
        do {
          from = std::clamp(X(eng), 2, x - 1);
        } while (jumps[from]);
        to = std::clamp(X(eng) % (x - from) + from + 1, from + 1, x);
      }

      jumps[from] = to;

      std::cout << from << ' ' << to << std::endl;
      if (verbose) {
        std::cerr << "ladder " << i << "\r";
      }
    }
    if (verbose) {
      std::cerr << std::endl;
    }

    std::cout << s << std::endl;
    for (int i = 0; i < s; i++) {
      int from, to;

      bool endless;

      if (no_cascade) {
        do {
          from = std::clamp(X(eng), 2, x - 1);
          to = std::clamp(X(eng) % from + 1, 1, from - 1);
        } while (jumps[from] || jumps[to]);
        jumps[to] = from;
      } else {
        do {
          from = std::clamp(X(eng), 2, x - 1);
          to = std::clamp(X(eng) % from + 1, 1, from - 1);
          endless = false;
          if (jumps[to]) {
            // check for endless loop
            std::unordered_set<int> visited;
            visited.insert(from);
            int next = to;
            while (next) {
              if (visited.contains(next)) {
                endless = true;
                break;
              }
              visited.insert(next);
              next = jumps[next];
            }
          }
        } while (endless);
      }

      jumps[from] = to;

      if (verbose) {
        if (!no_cascade && jumps[to]) {
          std::cerr << "Generated cascaded ladder-snake(s) in case " << t
                    << std::endl;

          std::unordered_set<int> visited;
          int next = from;
          while (next) {
            assert(!visited.contains(next));
            visited.insert(next);
            std::cerr << next;
            next = jumps[next];
            std::cerr << (next ? " -> " : "\n");
          }
        }
      }

      std::cout << from << ' ' << to << std::endl;
      if (verbose) {
        std::cerr << "snake " << i << "\r";
      }
    }

    if (verbose) {
      std::cerr << std::endl;
    }
  }
}
