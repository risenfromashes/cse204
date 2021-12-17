#include <catch2/catch.hpp>

#include "bstree.h"
#include <limits>
#include <random>

using namespace cse204;
TEST_CASE("BST operations", "[BSTree]") {
  BSTree<int> tree;

  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_int_distribution<int> dist(0, 99);

  SECTION("Insert/Delete/Find/In-Order") {
    int counts[100] = {0};

    for (int i = 0; i < 1000; i++) {
      int r = dist(rng);
      counts[r]++;
      tree.insert(r);
      REQUIRE(tree.find(r));
    }

    int size = 0;
    int last = std::numeric_limits<int>::min();

    tree.foreach (
        [&last, &size](int k) {
          REQUIRE(last <= k);
          last = k;
          size++;
        },
        TraversalOrder::IN_ORDER);

    REQUIRE(size == 1000);

    for (int i = 0; i < 2000; i++) {
      int r = dist(rng);
      std::cout << "r: " << r << " " << counts[r] << std::endl;
      if (counts[r]) {
        REQUIRE(tree.find(r));
        REQUIRE(tree.remove(r) == r);
        counts[r]--;
      } else {
        REQUIRE((!tree.find(r)));
      }

      int last = std::numeric_limits<int>::min();
      tree.foreach (
          [&last](int k) {
            REQUIRE(last <= k);
            last = k;
          },
          TraversalOrder::IN_ORDER);
    }
  }
}
