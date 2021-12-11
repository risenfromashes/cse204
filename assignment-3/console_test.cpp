#include <concepts>
#include <cstring>
#include <iostream>

#include "arrayqueue.h"
#include "linkedqueue.h"

#include "console_helper.h"

template <template <class, class> class queue_type>
requires cse204::ImplementsQueue<queue_type>
class QueueTester {
  typedef queue_type<int, std::allocator<int>> queue_t;
  queue_t  m_queue;

public:
  QueueTester(queue_t &&queue) : m_queue(std::move(queue)) {}

  int processInput(int a, int b) {
    switch (a) {
    case 1:
      m_queue.clear();
      return -1;
    case 2:
      m_queue.enqueue(b);
      return -1;
    case 3:
      try {
        return m_queue.dequeue();
      } catch (std::runtime_error e) {
        return -1;
      }
    case 4:
      return m_queue.length();
    case 5:
      try {
        return m_queue.frontValue();
      } catch (std::runtime_error e) {
        return -1;
      }
    case 6:
      try {
        return m_queue.rearValue();
      } catch (std::runtime_error e) {
        return -1;
      }
    case 7:
      try {
        return m_queue.leaveQueue();
      } catch (std::runtime_error e) {
        return -1;
      }
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
      m_queue.enqueue(v);
    }

    int a, b;
    std::cout << m_queue << std::endl;
    while (true) {
      std::cin >> a >> b;
      if (a == 0 && b == 0) {
        break;
      }
      int r = processInput(a, b);
      std::cout << m_queue << std::endl << r << std::endl;
    }
  }
};

int main(int argc, char **argv) {
  auto queue_type = selectQueueImplementation(argc, argv);
  if (queue_type.has_value()) {
    // select queue
    switch (queue_type.value()) {
    case QueueImplementationType::LINKED_QUEUE:
      QueueTester(cse204::LinkedQueue<int>()).test();
      break;
    case QueueImplementationType::ARRAY_QUEUE:
      QueueTester(cse204::ArrayQueue<int>()).test();
      break;
    }
  }

  return 0;
}
