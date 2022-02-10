#include <functional>
#include <iostream>
#include <vector>

#include "1.h"

std::string find_lcs(std::string x, std::string y) {
  int n = x.length();
  int m = y.length();

  int L[n + 1][m + 1];
  for (int i = 0; i <= n; i++)
    L[i][0] = 0;
  for (int i = 0; i <= m; i++)
    L[0][i] = 0;

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      L[i + 1][j + 1] =
          x[i] == y[i] ? L[i][j] + 1 : std::max(L[i + 1][j], L[i][j + 1]);
    }
  }

  std::string lcs;
  std::function<void(int, int)> trace_lcs = [&](int i, int j) {
    if (i && j) {
      if (L[i][j] == L[i - 1][j - 1] + 1) {
        trace_lcs(i - 1, j - 1);
        lcs.push_back(x[i - 1]);
      } else if (L[i][j] == L[i][j - 1]) {
        trace_lcs(i, j - 1);
      } else if (L[i][j] == L[i - 1][j]) {
        trace_lcs(i - 1, j);
      }
    }
  };
  trace_lcs(n, m);
  return lcs;
}

#ifndef TEST
int main() {
  std::string x, y;
  std::cin >> x;
  std::cin >> y;
  std::string lcs = find_lcs(x, y);
  std::cout << lcs.length() << std::endl;
  std::cout << lcs << std::endl;
}
#endif
