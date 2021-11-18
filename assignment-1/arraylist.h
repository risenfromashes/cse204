#pragma once

#include <algorithm>
#include <concepts>

#include "list.h"

namespace cse204 {
template <typename T> class arraylist : public list<T> {

  using typename list<T>::size_t;
  using list<T>::npos;

  static constexpr size_t k_default_capacity = 8;

  size_t m_capacity;
  size_t m_length;
  size_t m_pos;

  T *m_data;

public:
  /* creates empty list */
  arraylist()
      : m_capacity(k_default_capacity), m_length(0), m_pos(0),
        m_data(new T[m_capacity]) {}

  /* creates empty list with initial array capacity */
  arraylist(size_t initial_capacity)
      : m_capacity(initial_capacity), m_length(0), m_pos(0),
        m_data(new T[m_capacity]) {}

  /* creates list with data from static array */
  template <size_t N>
  arraylist(T const (&items)[N], size_t initial_capacity = k_default_capacity)
      : m_capacity(std::max(initial_capacity, N)), m_length(N), m_pos(0),
        m_data(new T[m_capacity]) {
    std::copy(items, items + N, m_data);
  }

  /* creates list with pointer data */
  arraylist(size_t num_items, T const *items,
            size_t initial_capacity = k_default_capacity)
      : m_capacity(std::max(initial_capacity, num_items)), m_length(num_items),
        m_pos(0), m_data(new T[m_capacity]) {
    std::copy(items, items + num_items, m_data);
  }

  /* copy constructor: copies elements from other list */
  arraylist(const arraylist<T> &other)
      : m_capacity(other.m_capacity), m_length(other.m_length),
        m_pos(other.m_pos), m_data(new T[m_capacity]) {
    std::copy(other.m_data, other.m_data + other.m_pos, m_data);
  }

  /* move constructor: steals elements from other list */
  arraylist(arraylist<T> &&other)
      : m_capacity(other.m_capacity), m_length(other.m_length),
        m_pos(other.m_pos), m_data(other.m_data) {
    /* Will resize to default capacity if inserted again */
    other.m_capacity = 0;
    other.m_length = 0;
    other.m_pos = 0;
    other.m_data = nullptr;
  }

  /* copy assignment: copies elements from other list */
  arraylist<T> &operator=(const arraylist<T> &other) {
    m_capacity = other.m_capacity;
    m_length = other.m_length;
    m_pos = other.m_pos;
    m_data = new T[m_capacity];
    std::copy(other.m_data, other.m_data + other.m_pos, m_data);
    return *this;
  }

  /* move assignment: steals elements from other list */
  arraylist<T> &operator=(arraylist<T> &&other) {
    m_capacity = other.m_capacity;
    m_length = other.m_length;
    m_pos = other.m_pos;
    m_data = other.m_data;
    other.m_capacity = 0;
    other.m_length = 0;
    other.m_pos = 0;
    other.m_data = nullptr;
  }

  /* destructor */
  ~arraylist() { delete[] m_data; }

private:
  /* helper methods */

  /* expands internal data array size by a factor of 2 by allocating new memory
   * and copying data to it */
  void expand() {
    auto old_data = m_data;
    if (m_capacity == 0) {
      // in case this arraylist was moved from
      m_capacity = k_default_capacity;
    }
    m_capacity *= 2;
    m_data = new T[m_capacity];
    std::copy(m_data, m_data + m_pos, old_data);
    delete[] old_data;
  }

public:
  /*  list interface implementation */

  /* Clears list, destroying all elements and emptying list. Leaves capacity
   * unchanged. */
  void clear() override {
    /* Destroy all elements. */
    for (size_t i = 0; i < m_length; i++) {
      if constexpr (std::destructible<T>) {
        m_data[i].~T();
      }
    }
    m_pos = m_length = 0;
  }

  /* Inserts element at current position. Expands if necessary. */
  void insert(const T &item) override {
    if (m_length >= m_capacity) {
      expand();
    }
    // shift data to the right
    for (size_t i = m_length; i > m_pos; i--) {
      m_data[i] = m_data[i - 1];
    }
    m_data[m_pos] = item;
    m_length++;
  }

  /* Appends an element at the end of the list. Expands if necessary. */
  void append(const T &item) override {
    if (m_length >= m_capacity) {
      expand();
    }
    m_data[m_length++] = item;
  }

  /* Remove element at current position and return it. */
  T remove() override {
    if (m_length == 0) {
      throw std::runtime_error("Attempt to remove from an empty list.");
    }
    if (m_pos >= m_length) {
      throw std::runtime_error(
          "Attempt to remove element from after the end of the list.");
    }
    T ret = m_data[m_pos];
    // shift data to the left
    for (size_t i = m_length - 1; i > m_pos; i--) {
      m_data[i - 1] = m_data[i];
    }
    m_length--;
    return ret;
  }

  /* Set the current position at the start of the list. */
  inline void moveToStart() override { m_pos = 0; }

  /* Set the current position at the end of the list. */
  inline void moveToEnd() override { m_pos = m_length; }

  /* Move the current position one step left unless already at the beginning. */
  inline void prev() override {
    if (m_pos > 0) {
      m_pos--;
    }
  }

  /* Move the current position one step right unless already at the end. */
  inline void next() override {
    if (m_pos < m_length) {
      m_pos++;
    }
  }

  /* Return the number of items in the list. */
  inline size_t length() const override { return m_length; }

  /* Return the current position of the list. */
  inline size_t currPos() const override { return m_pos; }

  /* Set current position. */
  inline void moveToPos(size_t pos) override {
    m_pos = pos;
    if (m_pos > m_length)
      m_pos = m_length;
  }

  /* Return the value of the current element */
  inline T getValue() const override {
    if (m_length == 0) {
      throw std::runtime_error("Attempting to access element from empty list.");
    }
    if (m_pos == m_length) {
      throw std::runtime_error(
          "Attempting to access element at the end of list.");
    }
    return m_data[m_pos];
  }

  /* Search for an item in the list, returns npos ( (size_t) -1 ) if not found. */
  size_t search(const T &item) const override {
    for (size_t i = 0; i < m_length; i++) {
      if (m_data[i] == item) {
        return i;
      }
    }
    return npos;
  }
};

} // namespace cse204
