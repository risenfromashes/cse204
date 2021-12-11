#pragma once

#include <cassert>
#include <concepts>
#include <cstddef>
#include <iostream>
#include <ostream>
#include <sstream>

namespace cse204 {

/* Abstract list interface */
template <class T, class Allocator = std::allocator<T>> class Queue {
protected:
  using size_t = typename Allocator::size_type;

public:
  /* List ADT methods */
  virtual void clear() = 0;
  virtual void enqueue(const T &item) = 0;
  virtual void enqueue(T &&item) = 0;
  virtual T dequeue() = 0;
  virtual size_t length() const = 0;
  virtual T &frontValue() = 0;
  virtual const T &frontValue() const = 0;
  virtual T &rearValue() = 0;
  virtual const T &rearValue() const = 0;
  virtual T leaveQueue() = 0;
};

template <template <class, class> class R>
concept ImplementsQueue =
    std::derived_from<R<int, std::allocator<int>>,
                      cse204::Queue<int, std::allocator<int>>>;

template <template <class, class> class queue_t, class T, class Alloc>
requires ImplementsQueue<queue_t> std::ostream &
operator<<(std::ostream &os, queue_t<T, Alloc> &queue) {
  queue_t<T, Alloc> temp = queue;
  queue.clear();
  os << '<';
  while (temp.length() > 0) {
    os << temp.frontValue();
    queue.enqueue(std::move(temp.dequeue()));
    if(temp.length()){
      os << ", ";
    }
  }
  os << '>';
  return os;
}

template <template <class, class> class queue_t, class T, class Alloc>
requires ImplementsQueue<queue_t> std::string
to_string(queue_t<T, Alloc> &queue) {
  std::ostringstream oss;
  oss << queue;
  return oss.str();
}

} // namespace cse204
