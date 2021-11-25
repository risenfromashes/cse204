#include <concepts>
#include <cstring>
#include <iostream>

#include "arraylist.h"
#include "linkedlist.h"

template <typename T, std::derived_from<cse204::list<T>> list_type>
class ListTester;

// deduction guides
template <template <typename> typename R, typename T>
ListTester(const R<T> &) -> ListTester<T, R<T>>;

template <template <typename> typename R, typename T>
ListTester(R<T> &&) -> ListTester<T, R<T>>;

template <typename T, std::derived_from<cse204::list<T>> list_type>
class ListTester {
private:
  list_type m_list;

public:
  ListTester(list_type &&list) : m_list(list) {}
  ListTester(const list_type &list) : m_list(list) {}

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
  // select list type
  bool ll = argc > 1 && (std::strcmp(argv[1], "-ll") == 0 ||
                         std::strcmp(argv[1], "--linkedlist") == 0);
  bool arr = argc > 1 && (std::strcmp(argv[1], "-arr") == 0 ||
                          std::strcmp(argv[1], "--arraylist") == 0);

  if (!(ll || arr)) {
    // show help
    std::cout << "Please provide one of the following flags:" << std::endl
              << std::endl
              << "-arr, --arraylist\tTo test the array based "
                 "implementation of the list interface"
              << std::endl
              << "-ll, --linkedlist\tTo test the linked list "
                 "implementation of the list interface"
              << std::endl
              << std::endl;
    return 0;
  }

  // input data
  int K, X;
  std::cin >> K >> X;
  int data[K];
  for (int i = 0; i < K; i++) {
    std::cin >> data[i];
  }

  // test list
  if (ll) {
    ListTester(cse204::linkedlist<int>(K, data)).test();
  } else if (arr) {
    ListTester(cse204::arraylist<int>(K, data, X)).test();
  }
  return 0;
}
