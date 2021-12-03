#include <concepts>
#include <cstring>
#include <iostream>

#include "arraystack.h"
#include "linkedstack.h"

#include "console_helper.h"

template <template <typename> typename stack_type>
requires ImplementsStack<stack_type>
class StackTester {
  stack_type<int> m_stack;

public:
  StackTester(stack_type<int> &&stack) : m_stack(stack) {}

  int processInput(int a, int b) {
    switch (a) {
    case 1:
      m_stack.clear();
      return -1;
    case 2:
      m_stack.push(b);
      return -1;
    case 3:
      return m_stack.pop();
    case 4:
      return m_stack.length();
    case 5:
      return m_stack.topValue();
    case 6:
      m_stack.setDirection(b);
      return -1;
    default:
      return -1;
    }
  }

  void test() {
    // input data
    int K;
    std::cin >> K;
    for (int i = 0; i < K; i++) {
      int v;
      std::cin >> v;
      m_stack.push(v);
    }

    int a, b;
    std::cout << m_stack << std::endl;
    while (true) {
      std::cin >> a >> b;
      if (a == 0 && b == 0) {
        break;
      }
      int r = processInput(a, b);
      std::cout << m_stack << std::endl << r << std::endl;
    }
  }
};

int main(int argc, char **argv) {
  auto stack_type = selectStackImplementation(argc, argv);
  if (stack_type.has_value()) {
    // select stack
    switch (stack_type.value()) {
    case StackImplementationType::LINKED_STACK:
      StackTester(cse204::LinkedStack<int>()).test();
      break;
    case StackImplementationType::ARRAY_STACK:
      StackTester(cse204::ArrayStack<int>()).test();
      break;
    case StackImplementationType::ARRAY2STACK:
      std::cout << "Array 2 Stack only applicable for the dishwasher problem"
                << std::endl;
      break;
    }
  }

  return 0;
}
