#include <algorithm>
#include <catch2/catch.hpp>
#include <iostream>
#include <limits>
#include <random>
#include <sstream>
#include <vector>

#include "sort.h"

std::vector<long> generate_random_vector() {
  static std::random_device dev;
  static std::mt19937 eng(dev());
  static std::uniform_int_distribution<long> dist(std::numeric_limits<long>::min(), std::numeric_limits<long>::max());
  int N = 1e3;
  std::vector<long> ret;
  for (int i = 0; i < N; i++) {
    ret.push_back(dist(eng));
  }
  return ret;
}

bool is_sorted(std::vector<long> &v) {
  for (size_t i = 1; i < v.size(); i++) {
    if (v[i] < v[i - 1]) {
      return false;
    }
  }
  return true;
}

std::string to_string(std::vector<long> &v) {
  std::ostringstream os;
  for (auto i : v) {
    os << i << ' ';
  }
  return os.str();
}

TEST_CASE("Testing sorting algorithms") {
  using cse204::PartitionScheme;

  SECTION("Insertion sort") {
    for (int i = 0; i < 1e3; i++) {
      auto a = generate_random_vector();
      auto b = a;
      cse204::insertion_sort(a);
      std::sort(b.begin(), b.end());
      REQUIRE(a == b);
    }
  }

  SECTION("Merge sort") {
    for (int i = 0; i < 1e3; i++) {
      auto a = generate_random_vector();
      auto b = a;
      cse204::merge_sort(a);
      std::sort(b.begin(), b.end());
      REQUIRE(a == b);
    }
  }

  SECTION("Quick sort (lomuto)") {
    for (int i = 0; i < 1e3; i++) {
      auto a = generate_random_vector();
      auto b = a;
      cse204::quick_sort<PartitionScheme::LOMUTO>(a);
      std::sort(b.begin(), b.end());
      REQUIRE(a == b);
    }
  }

  SECTION("Quick sort (hoare)") {
    for (int i = 0; i < 1e3; i++) {
      auto a = generate_random_vector();
      auto b = a;
      cse204::quick_sort<PartitionScheme::HOARE>(a);
      std::sort(b.begin(), b.end());
      REQUIRE(a == b);
    }
  }

  SECTION("Quick sort (lomuto random)") {
    for (int i = 0; i < 1e3; i++) {
      auto a = generate_random_vector();
      auto b = a;
      cse204::quick_sort<PartitionScheme::LOMUTO_RANDOM>(a);
      std::sort(b.begin(), b.end());
      REQUIRE(a == b);
    }
  }

  SECTION("Quick sort (hoare random)") {
    for (int i = 0; i < 1e3; i++) {
      auto a = generate_random_vector();
      auto b = a;
      cse204::quick_sort<PartitionScheme::HOARE_RANDOM>(a);
      std::sort(b.begin(), b.end());
      REQUIRE(a == b);
    }
  }
}
