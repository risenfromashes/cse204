#include <catch2/catch.hpp>

#include <iostream>

#include <limits>
#include <queue>
#include <random>
#include <vector>

#include "heap.h"

void test_heap(Heap &heap) {

  static std::random_device dev;
  static std::mt19937 rng(dev());
  static std::uniform_int_distribution<int> dist(-1000000, 1000000);

  std::priority_queue<int> pq;

  int t = 1e7;

  while (t--) {
    int r = dist(rng);

    if (dist(rng) % 1 && pq.size()) {
      pq.pop();
      int q = heap.extract_max();
    } else {
      pq.push(r);
      heap.insert(r);
    }
    REQUIRE(pq.size() == heap.size());
    REQUIRE(pq.top() == heap.top());
  }
}

TEST_CASE("Heap operations", "[BSTree]") {
  Heap heap;

  SECTION("Heap operations against priority queue") { test_heap(heap); }
}
