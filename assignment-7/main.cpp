#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <limits>

#include "sort.h"

auto generate_numbers(int n) {
  std::vector<int> ret(n);
  for (int i = 0; i < n; i++)
    ret[i] = cse204::random();
  return ret;
}

// return time required to perform task in milliseconds
float benchmark(auto &&task, int N, bool sorted = false, int n = 20) {
  std::srand(cse204::seed);
  float duration = 0.0;
  int t = n;
  while (t--) {
    auto data = generate_numbers(N);
    if (sorted) {
      std::sort(data.begin(), data.end());
    }
    auto start = std::chrono::high_resolution_clock::now();
    task(data);
    duration += (std::chrono::high_resolution_clock::now() - start).count() / 1e6;
  }
  return duration / n;
}

template <class... R, class T> void print_comma_seperated(std::ostream &os, T &&a, R &&...rest) {
  os << a;
  if constexpr (sizeof...(rest)) {
    os << ',';
    print_comma_seperated(os, rest...);
  } else {
    os << std::endl;
  }
}

void test_sorting_algorithms(std::ostream &out) {
  int N[] = {5, 10, 100, 1000, 10000, 100000};
  print_comma_seperated(out, ' ', ' ', ' ', ' ', "Time required in ms");
  print_comma_seperated(out, 'n', "Merge Sort", "Quicksort", "Randomized Quicksort", "Insertion Sort",
                        "Quicksort with Sorted Imput", "Randomized Quicksort with Sorted Imput", "STL sort() function");

  auto merge = [](auto &v) mutable { cse204::merge_sort(v); };
  auto quick = [](auto &v) mutable { cse204::quick_sort(v); };
  auto quick_rand = [](auto &v) mutable { cse204::quick_sort_random(v); };
  auto insert = [](auto &v) mutable { cse204::insertion_sort(v); };
  auto stl = [](auto &v) mutable { std::sort(v.begin(), v.end()); };

  for (int n : N) {
    print_comma_seperated(out, n, benchmark(merge, n), benchmark(quick, n), benchmark(quick_rand, n),
                          benchmark(insert, n), benchmark(quick, n, true), benchmark(quick_rand, n, true),
                          benchmark(stl, n));
  }
}
void test_lomuto() {}

int main() {
  std::ofstream file("benchmark.csv");
  test_sorting_algorithms(file);
  return 0;
}
