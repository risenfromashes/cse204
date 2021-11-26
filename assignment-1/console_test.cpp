#include <concepts>
#include <cstring>
#include <iostream>

#include "arraylist.h"
#include "linkedlist.h"

#include "console_helper.h"

#include <vector>

template <template <typename> typename list_type>
requires ImplementsList<list_type>
class ListTester {
  list_type<int> m_list;

public:
  ListTester(list_type<int> &&list) : m_list(list) {}

  int processInput(int a, int b) {
    switch (a) {
    case 1:
      m_list.clear();
      return -1;
    case 2:
      m_list.insert(b);
      return -1;
    case 3:
      m_list.append(b);
      return -1;
    case 4:
      return m_list.remove();
    case 5:
      m_list.moveToStart();
      return -1;
    case 6:
      m_list.moveToEnd();
      return -1;
    case 7:
      m_list.prev();
      return -1;
    case 8:
      m_list.next();
      return -1;
    case 9:
      return m_list.length();
    case 10:
      return m_list.currPos();
    case 11:
      m_list.moveToPos(b);
      return -1;
    case 12:
      return m_list.getValue();
    case 13:
      return m_list.Search(b);
    default:
      return -1;
    }
  }

  void test() {
    int a, b;
    std::cout << m_list << std::endl;
    while (true) {
      std::cin >> a >> b;
      if (a == 0 && b == 0) {
        break;
      }
      int r = processInput(a, b);
      std::cout << m_list << std::endl << r << std::endl;
    }
  }
};

int main(int argc, char **argv) {
  auto list_type = selectListImplementation(argc, argv);
  if (list_type.has_value()) {
    // input data
    int K, X;
    std::cin >> K >> X;
    int data[K];
    for (int i = 0; i < K; i++) {
      std::cin >> data[i];
    }
    // select list
    switch (list_type.value()) {
    case ListImplementationType::LINKED_LIST:
      ListTester(cse204::LinkedList<int>(K, data)).test();
      break;
    case ListImplementationType::ARRAY_LIST:
      ListTester(cse204::ArrayList<int>(K, data, X)).test();
      break;
    }
  }

  return 0;
}
