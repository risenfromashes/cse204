#include <catch2/catch.hpp>
#include <stdexcept>

#include "arraystack.h"
#include "linkedstack.h"

std::ostream &operator<<(std::ostream &os, const std::vector<int> &vec) {
  os << '<';
  for (int i = 0; i < vec.size(); i++) {
    os << vec[i];
    if (i < vec.size() - 1) {
      os << ", ";
    }
  }
  os << '>';
  return os;
}

TEMPLATE_PRODUCT_TEST_CASE("Basic tests", "[ArrayStack][LinkedStack]",
                           (cse204::ArrayStack, cse204::LinkedStack), (int)) {
  TestType stack = {20, 23, 12, 15};

  SECTION("Clear method should empty stack") {
    stack.clear();

    CHECK(stack.length() == 0);
    CHECK(cse204::to_string(stack) == "<>");
  }

  SECTION("Push should put element on top of stack") {
    stack.push(19);

    CHECK(stack.length() == 5);
    CHECK(cse204::to_string(stack) == "<20, 23, 12, 15, 19>");
    CHECK(stack.pop() == 19);
  }

  SECTION("Pop should remove element from the top of stack") {
    CHECK(stack.pop() == 15);
    CHECK(stack.length() == 3);
    CHECK(cse204::to_string(stack) == "<20, 23, 12>");
  }

  SECTION("Length should return size of stack") { CHECK(stack.length() == 4); }

  SECTION("TopValue should return element at the top of stack") {
    CHECK(stack.topValue() == 15);
    stack.pop();
    CHECK(stack.topValue() == 12);
    stack.push(19);
    CHECK(stack.topValue() == 19);
  }
}

TEMPLATE_PRODUCT_TEST_CASE("Bulk tests", "[ArrayStack][LinkedStack]",
                           (cse204::ArrayStack, cse204::LinkedStack),
                           (std::string)) {
  TestType stack;

  SECTION("Buld push/pop should work correctly") {
    for (int i = 0; i < 100; i++) {
      stack.push("String" + std::to_string(i));
      REQUIRE(stack.topValue() == ("String" + std::to_string(i)));
    }

    for (int i = 0; i < 50; i++) {
      REQUIRE(stack.topValue() == ("String" + std::to_string(99 - i)));
      REQUIRE(stack.pop() == ("String" + std::to_string(99 - i)));
      REQUIRE(stack.length() == (99 - i));
    }

    SECTION("Move constructor should work correctly") {
      TestType stack2 = std::move(stack);
      CHECK(stack2.length() == 50);
      for (int i = 0; i < 50; i++) {
        stack.push("String" + std::to_string(i + 50));
        stack2.push("String" + std::to_string(i + 50));
        REQUIRE(stack2.topValue() == ("String" + std::to_string(i + 50)));
      }
      CHECK(stack.length() == 50);
      for (int i = 0; i < 100; i++) {
        REQUIRE(stack2.topValue() == ("String" + std::to_string(99 - i)));
        REQUIRE(stack2.pop() == ("String" + std::to_string(99 - i)));
        REQUIRE(stack2.length() == (99 - i));
      }
    }

    SECTION("Move assignment should work correctly") {
      TestType stack2;
      stack2 = std::move(stack);
      CHECK(stack2.length() == 50);
      for (int i = 0; i < 50; i++) {
        stack.push("String" + std::to_string(i + 50));
        stack2.push("String" + std::to_string(i + 50));
        REQUIRE(stack2.topValue() == ("String" + std::to_string(i + 50)));
      }
      CHECK(stack.length() == 50);
      for (int i = 0; i < 100; i++) {
        REQUIRE(stack2.topValue() == ("String" + std::to_string(99 - i)));
        REQUIRE(stack2.pop() == ("String" + std::to_string(99 - i)));
        REQUIRE(stack2.length() == (99 - i));
      }
    }

    SECTION("Copy constructor should work correctly") {
      TestType stack2 = stack;
      CHECK(stack.length() == 50);
      CHECK(stack2.length() == 50);
      for (int i = 0; i < 50; i++) {
        stack2.push("String" + std::to_string(i + 50));
        REQUIRE(stack2.topValue() == ("String" + std::to_string(i + 50)));
      }
      for (int i = 0; i < 100; i++) {
        REQUIRE(stack2.topValue() == ("String" + std::to_string(99 - i)));
        REQUIRE(stack2.pop() == ("String" + std::to_string(99 - i)));
        REQUIRE(stack2.length() == (99 - i));
        if (i >= 50) {
          REQUIRE(stack.topValue() == ("String" + std::to_string(99 - i)));
          REQUIRE(stack.pop() == ("String" + std::to_string(99 - i)));
        }
      }
    }

    SECTION("Copy assignment should work correctly") {
      TestType stack2;
      stack2 = stack;
      CHECK(stack.length() == 50);
      CHECK(stack2.length() == 50);
      for (int i = 0; i < 50; i++) {
        stack2.push("String" + std::to_string(i + 50));
        REQUIRE(stack2.topValue() == ("String" + std::to_string(i + 50)));
      }
      for (int i = 0; i < 100; i++) {
        REQUIRE(stack2.topValue() == ("String" + std::to_string(99 - i)));
        REQUIRE(stack2.pop() == ("String" + std::to_string(99 - i)));
        if (i >= 50) {
          REQUIRE(stack.topValue() == ("String" + std::to_string(99 - i)));
          REQUIRE(stack.pop() == ("String" + std::to_string(99 - i)));
        }
      }
    }
  }

  SECTION("Reverse setDirection should work") {
    stack.setDirection(-1);
    for (int i = 0; i < 100; i++) {
      stack.push("String" + std::to_string(i));
      REQUIRE(stack.topValue() == ("String" + std::to_string(i)));
    }

    for (int i = 0; i < 50; i++) {
      REQUIRE(stack.topValue() == ("String" + std::to_string(99 - i)));
      REQUIRE(stack.pop() == ("String" + std::to_string(99 - i)));
    }
    stack.clear();

    CHECK(stack.length() == 0);
    CHECK(cse204::to_string(stack) == "<>");

    for (int i = 0; i < 100; i++) {
      stack.push("String" + std::to_string(i));
      REQUIRE(stack.topValue() == ("String" + std::to_string(i)));
    }

    for (int i = 0; i < 100; i++) {
      REQUIRE(stack.topValue() == ("String" + std::to_string(99 - i)));
      REQUIRE(stack.pop() == ("String" + std::to_string(99 - i)));
    }
  }
}

TEST_CASE("Stack should work correctly with non-owning memory",
          "[ArrayStack]") {
  auto *data = new std::string[100];
  cse204::ArrayStack<std::string> stack(100, data);

  SECTION("Buld push/pop should work correctly") {
    for (int i = 0; i < 100; i++) {
      stack.push("String" + std::to_string(i));
      REQUIRE(stack.topValue() == ("String" + std::to_string(i)));
    }

    for (int i = 0; i < 50; i++) {
      REQUIRE(stack.topValue() == ("String" + std::to_string(99 - i)));
      REQUIRE(stack.pop() == ("String" + std::to_string(99 - i)));
      REQUIRE(stack.length() == (99 - i));
    }

    SECTION("Move constructor should work correctly") {
      cse204::ArrayStack<std::string> stack2 = std::move(stack);
      CHECK(stack2.length() == 50);
      for (int i = 0; i < 50; i++) {
        stack2.push("String" + std::to_string(i + 50));
        REQUIRE(stack2.topValue() == ("String" + std::to_string(i + 50)));
      }
      for (int i = 0; i < 100; i++) {
        REQUIRE(stack2.topValue() == ("String" + std::to_string(99 - i)));
        REQUIRE(stack2.pop() == ("String" + std::to_string(99 - i)));
        REQUIRE(stack2.length() == (99 - i));
      }
    }

    SECTION("Move assignment should work correctly") {
      cse204::ArrayStack<std::string> stack2;
      stack2 = std::move(stack);
      CHECK(stack2.length() == 50);
      for (int i = 0; i < 50; i++) {
        stack2.push("String" + std::to_string(i + 50));
        REQUIRE(stack2.topValue() == ("String" + std::to_string(i + 50)));
      }
      for (int i = 0; i < 100; i++) {
        REQUIRE(stack2.topValue() == ("String" + std::to_string(99 - i)));
        REQUIRE(stack2.pop() == ("String" + std::to_string(99 - i)));
        REQUIRE(stack2.length() == (99 - i));
      }
    }

    SECTION("Copy constructor should work correctly") {
      cse204::ArrayStack<std::string> stack2 = stack;
      CHECK(stack.length() == 50);
      CHECK(stack2.length() == 50);
      for (int i = 0; i < 50; i++) {
        stack2.push("String" + std::to_string(i + 50));
        REQUIRE(stack2.topValue() == ("String" + std::to_string(i + 50)));
      }
      for (int i = 0; i < 100; i++) {
        REQUIRE(stack2.topValue() == ("String" + std::to_string(99 - i)));
        REQUIRE(stack2.pop() == ("String" + std::to_string(99 - i)));
        REQUIRE(stack2.length() == (99 - i));
        if (i >= 50) {
          REQUIRE(stack.topValue() == ("String" + std::to_string(99 - i)));
          REQUIRE(stack.pop() == ("String" + std::to_string(99 - i)));
        }
      }
    }

    SECTION("Copy assignment should work correctly") {
      cse204::ArrayStack<std::string> stack2;
      stack2 = stack;
      CHECK(stack.length() == 50);
      CHECK(stack2.length() == 50);
      for (int i = 0; i < 50; i++) {
        stack2.push("String" + std::to_string(i + 50));
        REQUIRE(stack2.topValue() == ("String" + std::to_string(i + 50)));
      }
      for (int i = 0; i < 100; i++) {
        REQUIRE(stack2.topValue() == ("String" + std::to_string(99 - i)));
        REQUIRE(stack2.pop() == ("String" + std::to_string(99 - i)));
        if (i >= 50) {
          REQUIRE(stack.topValue() == ("String" + std::to_string(99 - i)));
          REQUIRE(stack.pop() == ("String" + std::to_string(99 - i)));
        }
      }
    }
  }

  SECTION("Reverse setDirection should work") {
    stack.setDirection(-1);
    for (int i = 0; i < 100; i++) {
      stack.push("String" + std::to_string(i));
      REQUIRE(stack.topValue() == ("String" + std::to_string(i)));
    }

    for (int i = 0; i < 50; i++) {
      REQUIRE(stack.topValue() == ("String" + std::to_string(99 - i)));
      REQUIRE(stack.pop() == ("String" + std::to_string(99 - i)));
    }
    stack.clear();
    stack.setDirection(1);

    CHECK(stack.length() == 0);
    CHECK(cse204::to_string(stack) == "<>");

    for (int i = 0; i < 100; i++) {
      stack.push("String" + std::to_string(i));
      REQUIRE(stack.topValue() == ("String" + std::to_string(i)));
    }

    for (int i = 0; i < 100; i++) {
      REQUIRE(stack.topValue() == ("String" + std::to_string(99 - i)));
      REQUIRE(stack.pop() == ("String" + std::to_string(99 - i)));
    }
  }

  delete[] data;
}

// test no memory is leaked
TEMPLATE_PRODUCT_TEST_CASE("Non-trivial elements", "[ArrayStack][LinkedStack]",
                           (cse204::ArrayStack, cse204::LinkedStack),
                           (std::vector<int>)) {
  TestType stack;
  std::vector<std::vector<int>> vec, vec2;

  for (int i = 0; i < 20; i++) {
    vec.push_back(std::vector<int>{1 + i, 2 + i, 3 + i, 4 + i, 5 + i, 6 + i,
                                   7 + i, 8 + i, 9 + i, 10 + i});
  }

  vec2 = vec;

  SECTION("Push/Pop move should work correctly") {
    for (int i = 0; i < 20; i++) {
      stack.push(std::move(vec[i]));
    }
    for (int i = 0; i < 20; i++) {
      REQUIRE(stack.pop() == vec2[19 - i]);
    }
  }

  SECTION("Push/Pop copy should work correctly") {
    for (int i = 0; i < 20; i++) {
      stack.push(vec[i]);
    }
    for (int i = 0; i < 20; i++) {
      REQUIRE(stack.pop() == vec[19 - i]);
    }
  }

  SECTION("move/copy ctor should work") {
    for (int i = 0; i < 20; i++) {
      stack.push(vec[i]);
    }
    TestType stackc = stack;
    TestType stackm = std::move(stack);
    for (int i = 0; i < 20; i++) {
      REQUIRE(stackc.pop() == vec[19 - i]);
      REQUIRE(stackm.pop() == vec[19 - i]);
    }
  }

  SECTION("assignment should work") {
    TestType stackc, stackm;
    for (int i = 0; i < 20; i++) {
      stack.push(vec[i]);
    }
    stackc = stack;
    stackm = std::move(stack);
    for (int i = 0; i < 20; i++) {
      REQUIRE(stackc.pop() == vec[19 - i]);
      REQUIRE(stackm.pop() == vec[19 - i]);
    }
  }
}

// Test move-only type
TEMPLATE_PRODUCT_TEST_CASE("Move-only type", "[ArrayStack][LinkedStack]",
                           (cse204::ArrayStack, cse204::LinkedStack),
                           (std::unique_ptr<int>)) {
  TestType stack;
  SECTION("Push/Pop") {
    for (int i = 0; i < 10; i++) {
      stack.push(std::make_unique<int>(i));
    }
    for (int i = 0; i < 10; i++) {
      REQUIRE((*stack.pop()) == (9 - i));
    }
  }
  SECTION("Move ctor") {
    for (int i = 0; i < 10; i++) {
      stack.push(std::make_unique<int>(i));
    }
    TestType stackm = std::move(stack);
    for (int i = 0; i < 10; i++) {
      REQUIRE((*stackm.pop()) == (9 - i));
    }
  }
  SECTION("Move assignment") {
    TestType stackm;
    for (int i = 0; i < 10; i++) {
      stack.push(std::make_unique<int>(i));
    }
    stackm = std::move(stack);
    for (int i = 0; i < 10; i++) {
      REQUIRE((*stackm.pop()) == (9 - i));
    }
  }
}
