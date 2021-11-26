#include "arraylist.h"
#include "linkedlist.h"
#include <catch2/catch.hpp>
#include <stdexcept>

TEMPLATE_PRODUCT_TEST_CASE("List constructors", "[ArrayList][LinkedList]",
                           (cse204::ArrayList, cse204::LinkedList), int) {
  SECTION("Emtpy List Constructor") {
    TestType empty_list;

    CHECK(empty_list.length() == 0);
    CHECK(empty_list.currPos() == 0);

    empty_list.moveToStart();
    empty_list.moveToEnd();

    CHECK(empty_list.currPos() == 0);

    CHECK(cse204::to_string(empty_list) == "<>");
  }

  SECTION("Initialiser List Constructor") {
    TestType list({0, 1, 2, 3, 4});

    CHECK(list.length() == 5);
    CHECK(list.currPos() == 0);

    list.moveToPos(3);

    CHECK(cse204::to_string(list) == "<0 1 2 | 3 4>");

    list.moveToEnd();
    list.remove();
    CHECK(list.length() == 4);
    CHECK(list.currPos() == 3);
    CHECK(cse204::to_string(list) == "<0 1 2 | 3>");
  }

  SECTION("Static Array Constructor") {
    int data[] = {0, 1, 2, 3, 4};
    TestType list(data);

    CHECK(list.length() == 5);
    CHECK(list.currPos() == 0);

    list.moveToPos(2);

    CHECK(cse204::to_string(list) == "<0 1 | 2 3 4>");

    list.moveToStart();
    list.remove();
    CHECK(list.length() == 4);
    CHECK(list.currPos() == 0);
    CHECK(cse204::to_string(list) == "<| 1 2 3 4>");
  }

  SECTION("Pointer Array Constructor") {
    int *data = new int[5]{0, 1, 2, 3, 4};
    TestType list(5, data);

    CHECK(list.length() == 5);
    CHECK(list.currPos() == 0);

    list.moveToEnd();

    CHECK(cse204::to_string(list) == "<0 1 2 3 | 4>");

    list.moveToPos(2);
    list.remove();
    CHECK(list.length() == 4);
    CHECK(list.currPos() == 2);
    CHECK(cse204::to_string(list) == "<0 1 | 3 4>");
    delete[] data;
  }

  TestType other({0, 1, 2, 3, 4});
  other.moveToPos(1);

  SECTION("Copy Constructor: other list should be unchanged") {
    TestType list = other;

    CHECK(list.length() == 5);
    CHECK(list.currPos() == 1);
    CHECK(cse204::to_string(list) == "<0 | 1 2 3 4>");

    list.moveToEnd();
    CHECK(cse204::to_string(list) == "<0 1 2 3 | 4>");

    list.append(5);
    list.moveToEnd();
    CHECK(list.length() == 6);
    CHECK(list.currPos() == 5);
    CHECK(cse204::to_string(list) == "<0 1 2 3 4 | 5>");

    list.remove();
    CHECK(list.length() == 5);
    CHECK(list.currPos() == 4);
    CHECK(cse204::to_string(list) == "<0 1 2 3 | 4>");
  }

  SECTION("Move Constructor: other list should be empty") {
    TestType list = std::move(other);

    CHECK(list.length() == 5);
    CHECK(list.currPos() == 1);
    CHECK(cse204::to_string(list) == "<0 | 1 2 3 4>");

    list.append(5);
    CHECK(list.length() == 6);
    CHECK(cse204::to_string(list) == "<0 | 1 2 3 4 5>");

    list.moveToEnd();
    list.remove();
    CHECK(list.length() == 5);
    CHECK(cse204::to_string(list) == "<0 1 2 3 | 4>");

    CHECK(other.length() == 0);
    CHECK(other.currPos() == 0);
    CHECK(cse204::to_string(other) == "<>");

    other.append(0);
    other.append(1);
    other.append(2);
    other.moveToEnd();

    CHECK(other.length() == 3);
    CHECK(other.currPos() == 2);

    other.remove();
    CHECK(cse204::to_string(other) == "<0 | 1>");
  }
}

TEMPLATE_PRODUCT_TEST_CASE("Assignment Operators", "[ArrayList][LinkedList]",
                           (cse204::ArrayList, cse204::LinkedList), (int)) {
  TestType other({0, 1, 2, 3, 4});
  other.moveToPos(2);
  TestType list;

  SECTION("Copy Assignment: other list should be unchanged") {
    list = other;

    CHECK(list.length() == 5);
    CHECK(list.currPos() == 2);
    CHECK(cse204::to_string(list) == "<0 1 | 2 3 4>");

    list.insert(5);
    CHECK(list.length() == 6);
    CHECK(cse204::to_string(list) == "<0 1 | 5 2 3 4>");

    list.moveToPos(4);
    list.remove();
    CHECK(list.length() == 5);
    CHECK(list.currPos() == 4);
    CHECK(cse204::to_string(list) == "<0 1 5 2 | 4>");
  }

  SECTION("Move Assignment: other list should be empty") {
    list = std::move(other);
    CHECK(list.length() == 5);
    CHECK(list.currPos() == 2);
    CHECK(cse204::to_string(list) == "<0 1 | 2 3 4>");

    CHECK(other.length() == 0);
    CHECK(other.currPos() == 0);
    CHECK(cse204::to_string(other) == "<>");

    list.moveToPos(3);
    list.insert(5);
    CHECK(list.length() == 6);
    CHECK(cse204::to_string(list) == "<0 1 2 | 5 3 4>");

    other.append(5);
    other.append(6);
    other.append(7);
    other.append(8);
    other.moveToPos(2);
    CHECK(other.remove() == 7);
    CHECK(other.currPos() == 2);
    CHECK(cse204::to_string(other) == "<5 6 | 8>");
  }
}

TEMPLATE_PRODUCT_TEST_CASE("List Interface Methods", "[ArrayList][LinkedList]",
                           (cse204::ArrayList, cse204::LinkedList), (int)) {
  TestType list = {0, 1, 2, 3, 4};

  SECTION("1. clear: should empty list, but keep it valid") {
    list.clear();

    CHECK(list.length() == 0);
    CHECK(list.currPos() == 0);
    CHECK(cse204::to_string(list) == "<>");

    list.append(1);
    list.insert(0);
    list.next();

    CHECK(cse204::to_string(list) == "<0 | 1>");
  }

  SECTION("2. insert: should insert element at current position") {
    list.insert(-1);
    CHECK(cse204::to_string(list) == "<| -1 0 1 2 3 4>");

    list.moveToPos(2);
    list.insert(0);
    CHECK(cse204::to_string(list) == "<-1 0 | 0 1 2 3 4>");

    list.moveToPos(5);
    list.insert(0);
    CHECK(cse204::to_string(list) == "<-1 0 0 1 2 | 0 3 4>");

    list.moveToEnd();
    list.insert(0);
    CHECK(cse204::to_string(list) == "<-1 0 0 1 2 0 3 | 0 4>");
  }

  SECTION("3. append: should insert element at the end") {
    list.append(5);
    CHECK(cse204::to_string(list) == "<| 0 1 2 3 4 5>");

    list.moveToPos(3);
    list.append(6);
    CHECK(cse204::to_string(list) == "<0 1 2 | 3 4 5 6>");

    list.moveToEnd();
    list.append(7);
    CHECK(cse204::to_string(list) == "<0 1 2 3 4 5 | 6 7>");
  }

  SECTION("4. remove: should remove element at current position, moving back "
          "position if necessary") {
    CHECK(list.remove() == 0);
    CHECK(cse204::to_string(list) == "<| 1 2 3 4>");

    list.next();
    list.next();
    CHECK(list.remove() == 3);
    CHECK(cse204::to_string(list) == "<1 2 | 4>");

    list.prev();
    CHECK(list.remove() == 2);
    CHECK(cse204::to_string(list) == "<1 | 4>");

    CHECK(list.remove() == 4);
    CHECK(cse204::to_string(list) == "<| 1>");

    CHECK(list.remove() == 1);
    CHECK(cse204::to_string(list) == "<>");
  }

  SECTION("5. moveToStart: should set position in the beginning") {
    list.moveToEnd();
    list.moveToStart();
    CHECK(cse204::to_string(list) == "<| 0 1 2 3 4>");

    list.moveToEnd();
    list.remove();
    list.moveToStart();
    CHECK(cse204::to_string(list) == "<| 0 1 2 3>");

    list.clear();
    list.moveToStart();
    CHECK(list.currPos() == 0);
    CHECK(cse204::to_string(list) == "<>");
  }

  SECTION("6. moveToEnd: should set position in the beginning") {
    list.moveToEnd();
    CHECK(cse204::to_string(list) == "<0 1 2 3 | 4>");

    list.remove();
    list.prev();
    list.moveToEnd();
    CHECK(cse204::to_string(list) == "<0 1 2 | 3>");

    list.clear();
    list.moveToEnd();
    CHECK(list.currPos() == 0);
    CHECK(cse204::to_string(list) == "<>");
  }

  SECTION("7. prev: should set position one step to the left") {
    list.moveToEnd();
    list.prev();
    CHECK(list.currPos() == 3);
    list.prev();
    list.prev();
    CHECK(list.currPos() == 1);
    list.prev();
    CHECK(list.currPos() == 0);
    list.remove();
    list.prev();
    CHECK(list.currPos() == 0);
    list.next();
    list.prev();
    CHECK(list.currPos() == 0);
    list.clear();
    list.prev();
    CHECK(list.currPos() == 0);
  }

  SECTION("8. next: should set position one step to the right") {
    list.next();
    CHECK(list.currPos() == 1);
    list.next();
    list.next();
    CHECK(list.currPos() == 3);
    list.remove();
    list.next();
    CHECK(list.currPos() == 3);
    list.prev();
    list.next();
    CHECK(list.currPos() == 3);
    list.clear();
    list.next();
    CHECK(list.currPos() == 0);
  }

  SECTION("9. length: should return the number of elements") {
    CHECK(list.length() == 5);
    list.remove();
    CHECK(list.length() == 4);
    list.append(5);
    CHECK(list.length() == 5);
    list.clear();
    CHECK(list.length() == 0);
    list.insert(1);
    CHECK(list.length() == 1);
  }

  SECTION("10. currPos: should return the current position") {
    CHECK(list.currPos() == 0);
    list.moveToEnd();
    CHECK(list.currPos() == 4);
    list.moveToPos(3);
    CHECK(list.currPos() == 3);
    list.moveToStart();
    CHECK(list.currPos() == 0);
  }

  SECTION("11. moveToPos: should return the current position") {
    CHECK(list.currPos() == 0);
    list.moveToPos(2);
    CHECK(list.currPos() == 2);
    CHECK(cse204::to_string(list) == "<0 1 | 2 3 4>");
    list.moveToPos(3);
    CHECK(list.currPos() == 3);
    CHECK(cse204::to_string(list) == "<0 1 2 | 3 4>");
    list.remove();
    list.moveToPos(1);
    CHECK(list.currPos() == 1);
    CHECK(cse204::to_string(list) == "<0 | 1 2 4>");
  }

  SECTION("12. getValue: should return value of current element") {
    CHECK(list.getValue() == 0);
    list.moveToEnd();
    CHECK(list.getValue() == 4);
    list.remove();
    CHECK(list.getValue() == 3);
    list.insert(5);
    CHECK(list.getValue() == 5);
    list.next();
    CHECK(list.getValue() == 3);
    list.moveToStart();
    list.remove();
    CHECK(list.getValue() == 1);
  }

  SECTION("13. Search: should return position of an item in the list") {
    CHECK(list.Search(1) == 1);
    CHECK(list.Search(4) == 4);

    list.moveToPos(2);
    list.insert(5);
    list.append(7);

    CHECK(list.Search(0) == 0);

    list.moveToEnd();
    CHECK(list.Search(5) == 2);

    list.moveToStart();
    CHECK(list.Search(7) == 6);

    CHECK(list.Search(69) == -1);

    list.remove();
    CHECK(list.Search(0) == -1);
  }
}

TEMPLATE_PRODUCT_TEST_CASE("List of 100 elements", "[ArrayList][LinkedList]",
                           (cse204::ArrayList, cse204::LinkedList), (int)) {
  TestType list;
  for (int i = 99; i >= 0; i--) {
    list.insert(i);
    list.prev();
  }

  SECTION("Position should be at the start") { REQUIRE(list.currPos() == 0); }

  SECTION("Copy constructor should yield an equal list") {
    TestType listb = list;
    for (int i = 0; i < 100; i++, listb.next()) {
      REQUIRE(listb.getValue() == i);
    }
  }

  SECTION("Copy assignment should yield an equal list") {
    TestType listb = {0, 1, 2};
    listb = list;
    for (int i = 0; i < 100; i++, listb.next()) {
      REQUIRE(listb.getValue() == i);
    }
  }

  SECTION("Move constructor should yield a list equal list") {
    list.moveToPos(69);
    TestType listb = std::move(list);

    REQUIRE(listb.currPos() == 69);
    REQUIRE(listb.length() == 100);
    listb.moveToStart();
    for (int i = 0; i < 100; i++, listb.next()) {
      REQUIRE(listb.getValue() == i);
    }
  }

  SECTION("Move assignment should yield a list equal list") {
    list.moveToPos(69);
    TestType listb = {0, 1, 2, 3, 4};
    listb = std::move(list);

    REQUIRE(listb.currPos() == 69);
    REQUIRE(listb.length() == 100);
    listb.moveToStart();
    for (int i = 0; i < 100; i++, listb.next()) {
      REQUIRE(listb.getValue() == i);
    }
  }

  SECTION("Length should be set correctly") {
    REQUIRE(list.length() == 100);

    for (int i = 0; i < 95; i++) {
      list.remove();
    }

    REQUIRE(list.length() == 5);
  }

  SECTION("Append should yield same result") {
    TestType list2;
    for (int i = 0; i < 100; i++) {
      list2.append(i);
      list2.moveToPos(i);
    }
    REQUIRE(list2.length() == 100);
    for (int i = 99; i >= 0; i--, list2.prev()) {
      REQUIRE(list2.getValue() == i);
    }
  }

  SECTION("Values should match [next]") {
    for (int i = 0; i < 100; i++, list.next()) {
      REQUIRE(list.getValue() == i);
      REQUIRE(list.currPos() == i);
    }
  }

  SECTION("Values should match [prev]") {
    list.moveToEnd();
    for (int i = 99; i >= 0; i--, list.prev()) {
      REQUIRE(list.getValue() == i);
      REQUIRE(list.currPos() == i);
    }
  }

  SECTION("Remove should return elements removed") {
    for (int i = 0; i < 100; i++) {
      REQUIRE(list.remove() == i);
    }
  }

  SECTION("Remove should be able to remove elements at assigned positions") {
    // remove even elements
    for (int i = 0; i < 50; i++) {
      list.moveToPos(i);
      REQUIRE(list.remove() == (2 * i));
    }
    // remaining elements should be odd
    list.moveToEnd();
    for (int i = 50; i > 0; i--) {
      REQUIRE(list.remove() == (2 * i - 1));
    }
  }

  SECTION("Removing from end should set back position correctly") {
    TestType listb = std::move(list);
    listb.moveToEnd();
    for (int i = 99; i >= 0; i--) {
      REQUIRE(listb.currPos() == i);
      REQUIRE(listb.remove() == i);
    }
  }

  SECTION("Search should yield correct results") {
    REQUIRE(list.Search(49) == 49);
    REQUIRE(list.Search(99) == 99);
  }

  SECTION("List should be empty and properly freed when cleared") {
    list.clear();

    REQUIRE(list.length() == 0);
    REQUIRE(list.currPos() == 0);
  }
}
