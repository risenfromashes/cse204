#include <catch2/catch.hpp>
#include <cstdlib>
#include <pthread.h>
#include <stdexcept>

#include "arrayqueue.h"
#include "linkedqueue.h"

TEMPLATE_PRODUCT_TEST_CASE("Basic queue operations",
                           "[ArrayQueue][LinkedQueue]",
                           (cse204::ArrayQueue, cse204::LinkedQueue), (int)) {
  TestType queue = {1, 2, 3, 4, 5};

  SECTION("Test basic behaviour") {
    CHECK(queue.length() == 5);
    CHECK(queue.dequeue() == 1);
    CHECK(queue.leaveQueue() == 5);
    CHECK(queue.rearValue() == 4);
    CHECK(queue.frontValue() == 2);
    CHECK(queue.length() == 3);
    CHECK(cse204::to_string(queue) == "<2, 3, 4>");
    queue.clear();
    CHECK(queue.length() == 0);
  }

  SECTION("Bulk Insert Remove") {
    queue.clear();
    for (int k = 1; k <= 100; k++) {
      for (int i = 1; i <= k; i++) {
        queue.enqueue(i);
      }
      REQUIRE(queue.length() == k);
      for (int i = 1; i <= k; i++) {
        REQUIRE(queue.rearValue() == k);
        REQUIRE(queue.frontValue() == i);
        REQUIRE(queue.dequeue() == i);
      }
    }

    for (int k = 1; k <= 100; k++) {
      for (int i = 1; i <= k; i++) {
        queue.enqueue(i);
      }
      for (int i = k; i >= 1; i--) {
        REQUIRE(queue.frontValue() == 1);
        REQUIRE(queue.rearValue() == i);
        REQUIRE(queue.leaveQueue() == i);
      }
    }
  }

  SECTION("Copy ctor/assignment") {
    TestType qt = queue;

    CHECK(cse204::to_string(queue) == "<1, 2, 3, 4, 5>");
    CHECK(qt.dequeue() == 1);
    qt.enqueue(6);
    CHECK(qt.rearValue() == 6);
    qt.clear();
    CHECK(cse204::to_string(qt) == "<>");

    TestType q;
    for (int k = 5; k <= 100; k++) {
      qt.clear();
      for (int i = 1; i <= k; i++) {
        qt.enqueue(i);
      }

      if (std::rand() % 2 == 0) {
        TestType qt2 = qt;
        for (int i = 1; i <= k; i++) {
          REQUIRE(qt2.rearValue() == k);
          REQUIRE(qt2.frontValue() == i);
          REQUIRE(qt2.dequeue() == i);
        }
      } else {
        q = qt;
        for (int i = 1; i <= k; i++) {
          REQUIRE(q.rearValue() == k);
          REQUIRE(q.frontValue() == i);
          REQUIRE(q.dequeue() == i);
        }
      }
    }

    qt = queue;

    CHECK(cse204::to_string(queue) == "<1, 2, 3, 4, 5>");
    CHECK(qt.dequeue() == 1);
    qt.enqueue(6);
    CHECK(qt.rearValue() == 6);
    qt.clear();
    CHECK(cse204::to_string(qt) == "<>");
  }

  SECTION("Move ctor/assignment") {
    TestType q0, q1;
    q0 = q1 = queue;

    TestType qt = std::move(q0);

    CHECK(cse204::to_string(queue) == "<1, 2, 3, 4, 5>");
    CHECK(qt.dequeue() == 1);
    qt.enqueue(6);
    CHECK(qt.rearValue() == 6);
    qt.clear();
    CHECK(cse204::to_string(qt) == "<>");

    TestType q;
    for (int k = 5; k <= 100; k++) {
      q = std::move(qt);
      // moved from queue should be empty
      for (int i = 1; i <= k; i++) {
        qt.enqueue(i);
      }

      if (std::rand() % 2 == 0) {
        TestType qt2 = std::move(qt);
        for (int i = 1; i <= k; i++) {
          REQUIRE(qt2.rearValue() == k);
          REQUIRE(qt2.frontValue() == i);
          REQUIRE(qt2.dequeue() == i);
        }
      } else {
        q = std::move(qt);
        for (int i = 1; i <= k; i++) {
          REQUIRE(q.rearValue() == k);
          REQUIRE(q.frontValue() == i);
          REQUIRE(q.dequeue() == i);
        }
      }
    }

    qt = std::move(q1);
    CHECK(cse204::to_string(queue) == "<1, 2, 3, 4, 5>");
    CHECK(qt.dequeue() == 1);
    qt.enqueue(6);
    CHECK(qt.rearValue() == 6);
    qt.clear();
    CHECK(cse204::to_string(qt) == "<>");
  }
}

TEST_CASE("Non owning test", "[ArrayQueue]") {
  auto *data = new int[100];
  cse204::ArrayQueue<int> queue(100, data);
  cse204::ArrayQueue<int> qt;

  SECTION("Bulk test") {
    for (int k = 1; k <= 100; k++) {
      qt = std::move(queue);
      for (int i = 1; i <= k; i++) {
        qt.enqueue(i);
      }
      queue = std::move(qt);
      REQUIRE(queue.length() == k);
      for (int i = 1; i <= k; i++) {
        REQUIRE(queue.rearValue() == k);
        REQUIRE(queue.frontValue() == i);
        REQUIRE(queue.dequeue() == i);
      }
    }

    for (int k = 1; k <= 100; k++) {
      for (int i = 1; i <= k; i++) {
        queue.enqueue(i);
      }
      for (int i = k; i >= 1; i--) {
        REQUIRE(queue.frontValue() == 1);
        REQUIRE(queue.rearValue() == i);
        REQUIRE(queue.leaveQueue() == i);
        REQUIRE(queue.length() == (i - 1));
      }
    }
  }

  delete[] data;
}
