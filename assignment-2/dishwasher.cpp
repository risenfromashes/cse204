#include <iostream>
#include <limits>

#include "arraystack.h"
#include "linkedstack.h"

#include "console_helper.h"

struct Dish {
  int time_pushed;
  int size;
};

template <template <typename> typename stack_type>
requires ImplementsStack<stack_type>
class DishwasherSimulator {

  stack_type<Dish> m_dirty_stack;
  stack_type<Dish> m_clean_stack;

  int n, x;

  int time = 0;

public:
  DishwasherSimulator(stack_type<Dish> &&dirty_stack,
                      stack_type<Dish> &&clean_stack, int n, int x)
      : m_dirty_stack(std::move(dirty_stack)),
        m_clean_stack(std::move(clean_stack)), n(n), x(x) {}

  void cleanDish() {
    // start washing dish when cleaning last dish finished or the dish was
    // pushed, whichever happens later
    int start_time = std::max(m_clean_stack.length() > 0
                                  ? m_clean_stack.topValue().time_pushed + 1
                                  : std::numeric_limits<int>::min(),
                              m_dirty_stack.topValue().time_pushed);
    int size = m_dirty_stack.topValue().size;
    m_dirty_stack.topValue().time_pushed = start_time + size - 1;
    m_clean_stack.push(m_dirty_stack.pop());
  }

  void printCleanDishes() {
    stack_type<Dish> temp;
    while (m_clean_stack.length()) {
      temp.push(m_clean_stack.pop());
    }
    while (temp.length()) {
      std::cout << temp.topValue().time_pushed;
      m_clean_stack.push(temp.pop());
      if (temp.length()) {
        std::cout << ",";
      }
    }
  }

  void simulate() {
    stack_type<int> full_course_eaters;
    auto a = new int[x];

    for (int i = 0; i < x; i++) {
      std::cin >> a[i];
    }

    int k, t, s;
    while (true) {
      std::cin >> k >> t >> s;
      if (k == 0) {
        break;
      }
      // last course is eaten by full course eaters
      if (s == x) {
        full_course_eaters.push(k);
      }
      // If time_pushed >= t, then we are occupied cleaning last dish, so we
      // should let dishes stack up
      while (m_dirty_stack.length() &&
             (m_clean_stack.length() == 0 ||
              m_clean_stack.topValue().time_pushed < t)) {
        cleanDish();
      }
      // push dirty dish on stack
      m_dirty_stack.push({.time_pushed = t, .size = a[s - 1]});
    }
    // clean remaining dishes one after one
    while (m_dirty_stack.length() > 0) {
      cleanDish();
    }
    // when the last dish was cleaned all dishes are cleaned
    std::cout << m_clean_stack.topValue().time_pushed << std::endl;
    printCleanDishes();
    std::cout << std::endl;

    std::cout << "NY"[full_course_eaters.length() == n] << std::endl;

    while (full_course_eaters.length()) {
      std::cout << full_course_eaters.pop();
      if (full_course_eaters.length()) {
        std::cout << ',';
      }
    }
    std::cout << std::endl;

    delete[] a;
  }
};

int main(int argc, char **argv) {
  auto list_type = selectStackImplementation(argc, argv);
  if (list_type.has_value()) {
    // input n and x here, as they are required for array construction
    int n, x;
    std::cin >> n >> x;
    switch (list_type.value()) {
    case StackImplementationType::ARRAY_STACK:
      DishwasherSimulator(cse204::ArrayStack<Dish>(),
                          cse204::ArrayStack<Dish>(), n, x)
          .simulate();
      break;
    case StackImplementationType::LINKED_STACK:
      DishwasherSimulator(cse204::LinkedStack<Dish>(),
                          cse204::LinkedStack<Dish>(), n, x)
          .simulate();
      break;
    case StackImplementationType::ARRAY2STACK: {
      int max_dishes = n * x;
      auto *array = new Dish[max_dishes];
      DishwasherSimulator(cse204::ArrayStack<Dish>(max_dishes, array, 1),
                          cse204::ArrayStack<Dish>(max_dishes, array, -1), n, x)
          .simulate();
      delete[] array;
      break;
    }
    }
  }
}
