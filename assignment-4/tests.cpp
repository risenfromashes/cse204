#include <catch2/catch.hpp>

#include "bstree.h"
#include <limits>
#include <random>
#include <vector>

using namespace cse204;

void test_bst(BSTree<int> &tree, std::vector<int> counts) {
  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_int_distribution<int> dist(0, 99);

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

TEST_CASE("BST operations", "[BSTree]") {
  BSTree<int> tree;

  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_int_distribution<int> dist(0, 99);

  std::vector<int> counts(100, 0);

  for (int i = 0; i < 1000; i++) {
    int r = dist(rng);
    counts[r]++;
    tree.insert(r);
    REQUIRE(tree.find(r));
  }

  SECTION("Insert/Delete/Find/In-Order") { test_bst(tree, counts); }

  SECTION("Copy/move") {
    BSTree<int> copy = tree, copy2, copy3, copy4;
    copy2 = copy3 = copy4 = copy;
    test_bst(copy4, counts);
    test_bst(tree, counts);
    test_bst(copy, counts);
    BSTree<int> moved = std::move(copy2), moveassigned;
    test_bst(moved, counts);
    moveassigned = std::move(copy3);
    test_bst(moveassigned, counts);
  }
}
