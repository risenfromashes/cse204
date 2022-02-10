#include <cmath>
#include <iostream>
#include <vector>

#include "2.h"

constexpr long inf = std::numeric_limits<long>::max();

long find_min_travel_cost(int n, std::vector<std::vector<int>> &A) {
  unsigned n_mask = 1 << n;
  long C[n_mask][n];

  std::fill(C[0], C[0] + n_mask * n, inf);
  for (int i = 0; i < n; i++)
    C[1 << i][i] = A[i][i];

  for (unsigned m = 1; m < n_mask; m++) {
    for (int p = 0; p < n; p++) {
      if (m & (1 << p)) {
        for (int i = 0; i < n; i++) {
          if ((m & (1 << i)) == 0) { // previously unvisited
            long s = A[i][i];
            for (int j = 0; j < n; j++)
              s += A[i][j] * ((m & (1 << j)) != 0); // if previously visited
            C[m | (1 << i)][i] = std::min(C[m | (1 << i)][i], C[m][p] + s);
          }
        }
      }
    }
  }

  return *std::min_element(C[n_mask - 1], C[n_mask - 1] + n);
}

#ifndef TEST
int main() {
  int n;
  std::cin >> n;
  std::vector<std::vector<int>> A(n, std::vector<int>(n));

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      std::cin >> A[i][j];
    }
  }

  std::cout << find_min_travel_cost(n, A) << std::endl;
}
#endif
