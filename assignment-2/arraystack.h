#pragma once

#include <algorithm>
#include <concepts>
#include <memory>
#include <vector>

#include "stack.h"

namespace cse204 {
template <typename T> class ArrayStack : public Stack<T> {

  using typename Stack<T>::size_t;

  static constexpr size_t k_default_capacity = 8;

  size_t m_capacity;
  size_t m_length;

  T *m_data;

  bool m_owns_memory = true;
  int m_dir = 1;

public:
  /* creates empty stack */
  ArrayStack(size_t initial_capacity = k_default_capacity)
      : m_capacity(initial_capacity), m_length(0), m_data(new T[m_capacity]) {}

  /* Create stack from initializer stack */
  ArrayStack(std::initializer_list<T> items,
             size_t initial_capacity = k_default_capacity)
      : m_capacity(initial_capacity), m_length(items.size()) {
    fit_and_allocate();
    std::copy(items.begin(), items.end(), m_data);
  }

  /* Create stack from array, but does not own it */
  ArrayStack(size_t capacity, T *array, int dir = 1)
      : m_capacity(capacity), m_length(0), m_data(array), m_owns_memory(false) {
    if (dir != 1 && dir != -1) {
      throw std::runtime_error("Invalid direction");
    }
    m_dir = dir;
  }

  /* copy constructor: copies elements from other stack */
  ArrayStack(const ArrayStack &other) requires std::copyable<T>
      : m_capacity(other.m_capacity),
        m_length(other.m_length),
        m_data(new T[m_capacity]),
        m_owns_memory(true),
        m_dir(other.m_dir) {
    static_assert(std::copyable<T>);
    copy_from(other);
  }

  /* move constructor: steals elements from other stack */
  ArrayStack(ArrayStack &&other)
      : m_capacity(other.m_capacity), m_length(other.m_length),
        m_data(other.m_data), m_owns_memory(other.m_owns_memory),
        m_dir(other.m_dir) {
    /* Will resize to default capacity if inserted again */
    other.m_capacity = 0;
    other.m_length = 0;
    other.m_data = nullptr;
    other.m_owns_memory = true;
  }

  /* copy assignment: copies elements from other stack */
  ArrayStack &operator=(const ArrayStack &other) requires std::copyable<T> {
    if (this == &other) {
      return *this;
    }

    m_length = other.m_length;
    if (m_capacity < other.m_length) {
      // only delete and resize if array owns memory
      if (!m_owns_memory) {
        throw std::runtime_error(
            "Assignment exceeds capacity of array provided at construction");
      }
      delete[] m_data;
      fit_and_allocate();
    } else {
      destroy_elements();
    }

    copy_from(other);
    m_dir = other.m_dir;
    return *this;
  }

  /* move assignment: steals elements from other stack */
  ArrayStack &operator=(ArrayStack &&other) {
    if (this == &other) {
      return *this;
    }
    // exchange data and capacity, but setting other empty
    destroy_elements();
    m_length = other.m_length;
    m_dir = other.m_dir;

    std::swap(m_capacity, other.m_capacity);
    std::swap(m_data, other.m_data);
    std::swap(m_owns_memory, other.m_owns_memory);
    other.m_length = 0;

    return *this;
  }

  /* destructor */
  ~ArrayStack() {
    if (m_owns_memory) {
      delete[] m_data;
    }
  }

private:
  /* helper methods */

  /* Copies data from other stack, takes direction into account */
  void copy_from(const ArrayStack &other) {
    if (other.m_dir > 0) {
      std::copy(other.m_data, other.m_data + m_length, m_data);
    } else {
      std::copy(other.m_data + other.m_capacity - other.m_length,
                other.m_data + other.m_capacity, m_data);
    }
  }

  /* Increase capacity by chunk_size until it accomodates length, then allocate
   * to that capacity */
  void fit_and_allocate() {
    assert(m_owns_memory);
    while (m_capacity < m_length) {
      m_capacity *= 2;
    }
    m_data = new T[m_capacity];
  }

  /* expands internal data array size by the chuck size
   * and copying data to it */
  void expand() {
    if (!m_owns_memory) {
      throw std::runtime_error(
          "Operation exceeds capacity of array provided at construction");
    }
    if (m_capacity == 0) {
      m_capacity = k_default_capacity;
    }
    auto old_data = m_data;
    m_capacity *= 2;
    m_data = new T[m_capacity];
    if (m_dir > 0) {
      std::move(old_data, old_data + m_length, m_data);
    } else {
      std::move(old_data, old_data + m_length, m_data + m_length);
    }
    delete[] old_data;
  }

  /* destruct all elements */
  void destroy_elements() {
    if (m_dir > 0) {
      std::destroy(m_data, m_data + m_length);
    } else {
      std::destroy(m_data + m_capacity - m_length, m_data + m_capacity);
    }
  }

  /* returns position in the array of the element at the top of the stack */
  inline size_t array_pos() {
    return (m_length - 1) * m_dir + (m_dir < 0) * (m_capacity - 1);
  }

  /* returns position in the array of the next element at the top of the stack
   */
  inline size_t next_array_pos() {
    return m_length * m_dir + (m_dir < 0) * (m_capacity - 1);
  }

public:
  /* Stack interface implementation. */

  /* Clear contents from the stack, making it empty */
  void clear() override {
    destroy_elements();
    m_length = 0;
  }

  /* Pushes item on the top of the stack */
  void push(const T &item) override {
    if constexpr (std::copyable<T>) {
      if (m_length >= m_capacity) {
        expand();
      }
      std::construct_at(m_data + next_array_pos(), item);
      m_length++;
    }
  }

  /* Pushes item on the top of the stack */
  void push(T &&item) override {
    if (m_length >= m_capacity) {
      expand();
    }
    std::construct_at(m_data + next_array_pos(), std::move(item));
    m_length++;
  }

  /* Pops item from the top of the stack and returns value */
  T pop() override {
    if (m_length == 0) {
      throw std::runtime_error("Attempt to pop from empty stack");
    }
    /* move object if move constructor is available */
    T ret = [this]() constexpr {
      if constexpr (std::move_constructible<T>) {
        return std::move(m_data[array_pos()]);
      } else {
        return m_data[array_pos()];
      }
    }
    ();
    std::destroy_at(m_data + array_pos());
    m_length--;
    return ret;
  }

  /* Returns size of the stack */
  inline size_t length() const override { return m_length; }

  /* Returns the value of the top element of the stack */
  inline T &topValue() override {
    if (m_length == 0) {
      throw std::runtime_error("Attempt to get top value from empty stack");
    }
    return m_data[array_pos()];
  }

  /* Returns the value of the top element of the stack */
  inline const T &topValue() const override { return topValue(); }

  /* Sets the direction in which elements of the stack are inserted */
  void setDirection(int dir) override {
    if (m_length > 0) {
      throw std::runtime_error("Attempt to set direction in non-empty stack");
    }
    if (dir != 1 && dir != -1) {
      throw std::runtime_error("Invalid direction");
    }
    m_dir = dir;
  }
};

#ifndef CSE204_STACK_OUTPUT_STREAM_IMPLEMENTED
#define CSE204_STACK_OUTPUT_STREAM_IMPLEMENTED
template <typename T>
std::ostream &operator<<(std::ostream &os, Stack<T> &stack) {
  ArrayStack<T> temp;
  while (stack.length() > 0) {
    temp.push(std::move(stack.pop()));
  }
  os << '<';
  while (temp.length() > 0) {
    T v = temp.pop();
    os << v;
    if (temp.length() > 0) {
      os << ", ";
    }
    stack.push(std::move(v));
  }
  os << '>';
  return os;
}
#endif

} // namespace cse204
