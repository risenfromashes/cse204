#pragma once

#include <algorithm>
#include <concepts>
#include <memory>

#include "queue.h"

namespace cse204 {
template <class T, class Allocator = std::allocator<T>>
class ArrayQueue : public Queue<T, Allocator> {

  using size_t = typename Queue<T, Allocator>::size_t;
  using allocator_traits = std::allocator_traits<Allocator>;

  static constexpr size_t k_default_capacity = 8;

  Allocator m_allocator;

  size_t m_capacity;
  size_t m_length;
  size_t m_front;

  T *m_data;

  bool m_owns_memory = true;

public:
  /* creates empty queue */
  ArrayQueue(size_t initial_capacity = k_default_capacity) noexcept
      : m_capacity(initial_capacity), m_length(0), m_front(0),
        m_data(allocator_traits::allocate(m_allocator, m_capacity)) {}

  /* Create queue from initializer list */
  ArrayQueue(std::initializer_list<T> items,
             size_t initial_capacity = k_default_capacity)
      : m_capacity(initial_capacity), m_length(items.size()), m_front(0) {
    fit_and_allocate();
    std::uninitialized_move(items.begin(), items.end(), m_data);
  }

  /* Create queue from array, but does not own it */
  ArrayQueue(size_t capacity, T *array)
      : m_capacity(capacity), m_length(0), m_front(0), m_data(array),
        m_owns_memory(false) {}

  /* copy constructor: copies elements from other queue */
  ArrayQueue(const ArrayQueue &other) requires std::copyable<T>
      : m_capacity(other.m_capacity),
        m_length(other.m_length),
        m_data(allocator_traits::allocate(m_allocator, m_capacity)),
        m_owns_memory(true) {
    copy_from(other);
  }

  /* move constructor: steals elements from other queue */
  ArrayQueue(ArrayQueue &&other)
      : m_capacity(other.m_capacity), m_length(other.m_length),
        m_front(other.m_front), m_data(other.m_data),
        m_owns_memory(other.m_owns_memory) {
    /* Will resize to default capacity if inserted again */
    other.m_capacity = 0;
    other.m_length = 0;
    other.m_front = 0;
    other.m_data = nullptr;
    other.m_owns_memory = true;
  }

  /* copy assignment: copies elements from other queue */
  ArrayQueue &operator=(const ArrayQueue &other) requires std::copyable<T> {
    if (this == &other) {
      return *this;
    }

    if (m_capacity < other.m_length) {
      // only delete and resize if array owns memory
      if (!m_owns_memory) {
        throw std::runtime_error(
            "Assignment exceeds capacity of array provided at construction");
      }
      // deallocate current memory
      destroy_elements();
      allocator_traits::deallocate(m_allocator, m_data, m_capacity);
      // allocate new memory to fit data
      m_length = other.m_length;
      fit_and_allocate();
    } else {
      destroy_elements();
    }

    copy_from(other);
    return *this;
  }

  /* move assignment: steals elements from other queue */
  ArrayQueue &operator=(ArrayQueue &&other) {
    if (this == &other) {
      return *this;
    }
    // exchange data and capacity, but setting other empty
    destroy_elements();
    m_length = other.m_length;
    m_front = other.m_front;

    std::swap(m_capacity, other.m_capacity);
    std::swap(m_data, other.m_data);
    std::swap(m_owns_memory, other.m_owns_memory);

    other.m_length = 0;

    return *this;
  }

  /* destructor */
  ~ArrayQueue() {
    if (m_owns_memory) {
      allocator_traits::deallocate(m_allocator, m_data, m_capacity);
    }
  }

private:
  /* helper methods */

  /* Returns position of the front element in the array */
  inline size_t front_pos() const {
    assert(m_capacity > 0);
    assert(m_length <= m_capacity);
    return m_front;
  }

  /* Returns position of the rear element in the array */
  inline size_t rear_pos() const {
    assert(m_capacity > 0);
    assert(m_length <= m_capacity);
    return (m_front + m_length - 1) % m_capacity;
  }

  /* Copies elements from other queue, sets m_front accordingly */
  void copy_from(const ArrayQueue &other) {
    assert(other.m_length <= m_capacity);
    m_length = other.m_length;
    if (m_length) {
      // copy from front to rear or end of data
      auto other_front = other.front_pos();
      auto other_rear = other.rear_pos() + 1;

      if (other_front < other_rear) {
        // if rear is ahead of front, the queue doesn't come back around the
        // array so we are done with just one copy
        //
        // |[0]...........[rear]-----(1)----[front]...........[capacity]|
        std::uninitialized_copy(other.m_data + other_front,
                                other.m_data + other_rear, m_data);
      } else {
        // it does come back around in this case, so two copy needed
        //
        // |[0]----(2)-----[rear]..........[front]----(1)-----[capacity]|
        std::uninitialized_copy(other.m_data + other_front,
                                other.m_data + other.m_capacity, m_data); // (1)
        std::uninitialized_copy(other.m_data, other.m_data + other_rear,
                                m_data +
                                    (other.m_capacity - other_front)); // (2)
      }
      m_front = 0;
    }
  }

  /* Increase capacity by chunk_size until it accomodates length, then allocate
   * to that capacity */
  void fit_and_allocate() {
    assert(m_owns_memory);
    while (m_capacity < m_length) {
      m_capacity *= 2;
    }
    m_data = allocator_traits::allocate(m_allocator, m_capacity);
  }

  /* expands internal data array size by a factor of 2
   * and copying old data to it */
  void expand() {
    if (!m_owns_memory) {
      throw std::runtime_error(
          "Operation exceeds capacity of array provided at construction");
    }

    if (m_capacity == 0) {
      // surely a queue with zero capacity cannot contain anything
      assert(m_length == 0);
      m_capacity = k_default_capacity;
      m_data = allocator_traits::allocate(m_allocator, m_capacity);
      m_front = 0;
      return;
    }

    auto new_capacity = m_capacity * 2;
    auto new_data = allocator_traits::allocate(m_allocator, new_capacity);

    if (m_length) {
      auto front = front_pos();
      auto rear = rear_pos() + 1;
      // see copy_from
      if (front < rear) {
        std::uninitialized_move(m_data + front, m_data + rear, new_data); // 1
      } else {
        std::uninitialized_move(m_data + front, m_data + m_capacity,
                                new_data); // (1)
        std::uninitialized_move(m_data, m_data + rear,
                                new_data + (m_capacity - front)); // (2)
      }
      // delete old memory
      destroy_elements();
      allocator_traits::deallocate(m_allocator, m_data, m_capacity);
    }
    // start using new memory
    // allocations have been made in a way that new queue always starts from 0
    m_capacity = new_capacity;
    m_data = new_data;
    m_front = 0;
  }

  /* destruct all elements */
  void destroy_elements() {
    if (m_length > 0) {
      auto front = front_pos();
      auto rear = rear_pos();
      for (size_t i = front;; i = (i + 1) % m_capacity) {
        allocator_traits::destroy(m_allocator, m_data + i);
        if (i == rear) {
          break;
        }
      }
    }
  }

  /* construct element in place in the queue */
  template <class... R>
  requires std::constructible_from<T, R...>
  void emplace(R &&...params) {
    if (m_length >= m_capacity) {
      expand();
    }
    m_length++;
    allocator_traits::construct(m_allocator, m_data + rear_pos(),
                                std::forward<R>(params)...);
  }

public:
  /* Queue interface implementation. */

  /* Clear contents from the queue, making it empty */
  void clear() override {
    destroy_elements();
    m_length = 0;
  }

  /* Enqueue a copy of item at the back of the queue */
  void enqueue(const T &item) override { emplace(item); }

  /* Enqueue by moving item at the back of the queue */
  void enqueue(T &&item) override { emplace(std::move(item)); }

  /* Dequeues an item from the front of the queue and returns value */
  T dequeue() override {
    if (m_length == 0) {
      throw std::runtime_error("Attempt to dequeue from empty queue");
    }
    auto front = front_pos();
    T ret = std::move(m_data[front]);
    /* move object if move constructor is available */
    allocator_traits::destroy(m_allocator, m_data + front);
    m_front = (m_front + 1) % m_capacity;
    m_length--;
    return ret;
  }

  /* Returns size of the queue */
  inline size_t length() const override { return m_length; }

  /* Returns the value of the front element of the queue */
  inline T &frontValue() override {
    if (m_length == 0) {
      throw std::runtime_error("Attempt to get front value from empty queue");
    }
    return m_data[front_pos()];
  }

  /* Returns the value of the front element of the queue */
  inline const T &frontValue() const override { return frontValue(); }

  /* Returns the value of the rear element of the queue */
  inline T &rearValue() override {
    if (m_length == 0) {
      throw std::runtime_error("Attempt to get rear value from empty queue");
    }
    return m_data[rear_pos()];
  }

  /* Returns the value of the rear element of the queue */
  inline const T &rearValue() const override { return rearValue(); }

  /* Returns the value of the top element of the queue */
  T leaveQueue() override {
    if (m_length == 0) {
      throw std::runtime_error("Attempt to dequeue from empty queue");
    }
    auto rear = rear_pos();
    T ret = std::move(m_data[rear]);
    allocator_traits::destroy(m_allocator, m_data + rear);
    m_length--;
    return ret;
  }
};

} // namespace cse204
