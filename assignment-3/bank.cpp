#include "arrayqueue.h"
#include "console_helper.h"
#include "linkedqueue.h"

#include <iomanip>
#include <iostream>
#include <limits>

bool g_debug_print = false;

struct Customer {
  inline static int count = 0;
  int index = -1;
  int entry_time = -1;
  int service_time = -1;

  Customer() {}
  Customer(int entry, int service)
      : index(++count), entry_time(entry), service_time(service) {}

  friend std::ostream &operator<<(std::ostream &os, Customer &c) {
    if (c.index > 0) {
      os << "C" << c.index;
    }
    return os;
  }
};

struct Booth {
  int busy_until = 0;
  Customer customer;

  bool is_busy(int time) { return time < busy_until; }
  void serveCustomer(int time, Customer customer_) {
    assert(!is_busy(time));
    customer = customer_;
    busy_until = time + customer.service_time;
  }
};

template <template <class, class> class queue_type>
requires cse204::ImplementsQueue<queue_type>
class Bank {
  typedef queue_type<Customer, std::allocator<Customer>> queue_t;

  int time = 0;
  queue_t queues[2];
  Booth booths[2];

public:
  Bank() {}

  void serveCustomer() {}

  void switchQueues() {
    if ((queues[0].length() + 1) < queues[1].length()) {
      std::cout << "qs" << std::endl;
      queues[0].enqueue(queues[1].leaveQueue());
    } else if ((queues[1].length() + 1) < queues[0].length()) {
      queues[1].enqueue(queues[0].leaveQueue());
      std::cout << "qs" << std::endl;
    }
  }
  void printState() {
    if (g_debug_print) {
      std::cout << time << " ";
      std::cout << queues[0] << " " << queues[1] << " ";
      if (booths[0].is_busy(time)) {
        std::cout << booths[0].customer;
      }
      std::cout << " ";
      if (booths[1].is_busy(time)) {
        std::cout << booths[1].customer;
      }
      std::cout << std::endl;
    }
  }
  // run simulation assuming no new customers have come
  void elapse(int t_e) {
    while (time < t_e) {
      // prioritise dequeueing
      for (int i = 0; i < 2; i++) {
        if (!booths[i].is_busy(time)) {
          if (queues[i].length() > 0) {
            booths[i].serveCustomer(time, queues[i].dequeue());
          }
        }
      }
      // then switch directly from other queue to be served
      for (int i = 0; i < 2; i++) {
        if (!booths[i].is_busy(time)) {
          if (queues[i ^ 1].length() > 0) {
            booths[i].serveCustomer(time, queues[i ^ 1].dequeue());
          }
        }
      }
      // then regular switch
      switchQueues();
      printState();
      time++;
      if (queues[0].length() == 0 && queues[1].length() == 0 &&
          !booths[0].is_busy(time) && !booths[1].is_busy(time)) {
        break;
      }
    }
  }

  void process() {
    int n;
    std::cin >> n;

    int t, s;
    while (n--) {
      std::cin >> t >> s;
      elapse(t);
      Customer c{t, s};
      // if either booth is empty serve directly
      int served = false;
      for (int i = 0; i < 2; i++) {
        if (queues[i].length() == 0 && !booths[i].is_busy(t)) {
          booths[i].serveCustomer(t, c);
          served = true;
          break;
        }
      }
      if (!served) {
        // otherwise queue to shorter queue
        if (queues[0].length() < queues[1].length()) {
          queues[0].enqueue(c);
        } else {
          queues[1].enqueue(c);
        }
      }
      printState();
    }
    elapse(std::numeric_limits<int>::max());
    std::cout << "Booth 1 finishes service at t=" << booths[0].busy_until
              << std::endl;
    std::cout << "Booth 2 finishes service at t=" << booths[1].busy_until
              << std::endl;
  }
};

int main(int argc, char **argv) {
  g_debug_print = argc >= 3 && std::strcmp(argv[2], "-D") == 0;
  auto queue_type = selectQueueImplementation(argc, argv);
  if (queue_type.has_value()) {
    // select queue
    switch (queue_type.value()) {
    case QueueImplementationType::LINKED_QUEUE:
      Bank<cse204::LinkedQueue>().process();
      break;
    case QueueImplementationType::ARRAY_QUEUE:
      Bank<cse204::ArrayQueue>().process();
      break;
    }
  }

  return 0;
}
