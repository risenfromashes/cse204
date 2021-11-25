#pragma once

#include <algorithm>
#include <concepts>

#include "list.h"

namespace cse204 {
template <typename T> class arraylist : public list<T> {

  using typename list<T>::size_t;
  using list<T>::npos;

  static constexpr size_t k_default_chunk_size = 8;

  size_t m_chunk_size;
  size_t m_capacity;
  size_t m_length;
  size_t m_pos;

  T *m_data;

/* check validity of state */
#define CHECK_STATE()                                                          \
  if (m_length > 0) {                                                          \
    assert(m_length <= m_capacity);                                            \
    assert(m_pos < m_length);                                                  \
    assert(m_data);                                                            \
  } else {                                                                     \
    assert(m_pos == 0);                                                        \
  }

public:
  /* creates empty list */
  arraylist(size_t chunk_size = k_default_chunk_size)
      : m_chunk_size(k_default_chunk_size), m_capacity(m_chunk_size),
        m_length(0), m_pos(0), m_data(new T[m_capacity]) {
          CHECK_STATE();
        }

  /* Create list from initializer list */
  arraylist(std::initializer_list<T> items,
            size_t chunk_size = k_default_chunk_size)
      : m_chunk_size(chunk_size), m_capacity(m_chunk_size),
        m_length(items.size()), m_pos(0) {
    fit_and_allocate();
    std::copy(items.begin(), items.end(), m_data);
    CHECK_STATE();
  }
  /* Creates list with data from static array */
  template <size_t N>
  arraylist(T const (&items)[N], size_t chunk_size = k_default_chunk_size)
      : m_chunk_size(chunk_size), m_capacity(m_chunk_size), m_length(N),
        m_pos(0) {
    fit_and_allocate();
    std::copy(items, items + N, m_data);
    CHECK_STATE();
  }

  /* creates list with pointer data */
  arraylist(size_t num_items, T const *items,
            size_t chunk_size = k_default_chunk_size)
      : m_chunk_size(chunk_size), m_capacity(m_chunk_size), m_length(num_items),
        m_pos(0) {
    fit_and_allocate();
    std::copy(items, items + num_items, m_data);
    CHECK_STATE();
  }

  /* copy constructor: copies elements from other list */
  arraylist(const arraylist<T> &other)
      : m_chunk_size(other.m_chunk_size), m_capacity(other.m_capacity),
        m_length(other.m_length), m_pos(other.m_pos),
        m_data(new T[m_capacity]) {
    std::copy(other.m_data, other.m_data + other.m_length, m_data);
    CHECK_STATE();
  }

  /* move constructor: steals elements from other list */
  arraylist(arraylist<T> &&other)
      : m_chunk_size(other.m_chunk_size), m_capacity(other.m_capacity),
        m_length(other.m_length), m_pos(other.m_pos), m_data(other.m_data) {
    /* Will resize to default capacity if inserted again */
    other.m_chunk_size = k_default_chunk_size;
    other.m_capacity = 0;
    other.m_length = 0;
    other.m_pos = 0;
    other.m_data = nullptr;
    CHECK_STATE();
  }

  /* copy assignment: copies elements from other list */
  arraylist<T> &operator=(const arraylist<T> &other) {
    if (this == &other) {
      return *this;
    }
    destroy_elements();
    m_length = other.m_length;
    m_pos = other.m_pos;
    if (m_capacity < other.m_length) {
      delete[] m_data;
      fit_and_allocate();
    }
    std::copy(other.m_data, other.m_data + other.m_length, m_data);

    CHECK_STATE();
    return *this;
  }

  /* move assignment: steals elements from other list */
  arraylist<T> &operator=(arraylist<T> &&other) {
    // exchange data and capacity, but setting other empty
    destroy_elements();
    m_length = other.m_length;
    m_pos = other.m_pos;
    std::swap(m_chunk_size, other.m_chunk_size);
    std::swap(m_capacity, other.m_capacity);
    std::swap(m_data, other.m_data);
    other.m_pos = other.m_length = 0;

    CHECK_STATE();
    return *this;
  }

  /* destructor */
  ~arraylist() { delete[] m_data; }

private:
  /* helper methods */

  /* Increase capacity by chunk_size until it accomodates length, then allocate
   * to that capacity */
  void fit_and_allocate() {
    while (m_capacity < m_length) {
      m_capacity += m_chunk_size;
    }
    m_data = new T[m_capacity];
  }

  /* expands internal data array size by the chuck size
   * and copying data to it */
  void expand() {
    if (m_chunk_size == 0) {
      m_chunk_size = k_default_chunk_size;
    }
    auto old_data = m_data;
    m_capacity += m_chunk_size;
    m_data = new T[m_capacity];
    std::copy(old_data, old_data + m_length, m_data);
    delete[] old_data;
  }

  /* destruct all elements */
  void destroy_elements() {
    for (size_t i = 0; i < m_length; i++) {
      if constexpr (std::destructible<T>) {
        m_data[i].~T();
      }
    }
  }

public:
  /*  list interface implementation */

  /* Clears list, destroying all elements and emptying list. Leaves capacity
   * unchanged. */
  void clear() override {
    CHECK_STATE();

    destroy_elements();
    m_pos = m_length = 0;
  }

  /* Inserts element at current position. Expands if necessary. */
  void insert(const T &item) override {
    CHECK_STATE();

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
    CHECK_STATE();

    if (m_length >= m_capacity) {
      expand();
    }
    m_data[m_length++] = item;
  }

  /* Remove element at current position and return it. */
  T remove() override {
    CHECK_STATE();

    if (m_length == 0) {
      throw std::runtime_error("Attempt to remove from an empty list.");
    }

    assert(m_pos < m_length);
    T ret = m_data[m_pos];
    // shift data to the left
    for (size_t i = m_pos; i < m_length - 1; i++) {
      m_data[i] = m_data[i + 1];
    }
    m_length--;
    if (m_pos == m_length && m_pos > 0) {
      m_pos--;
    }
    return ret;
  }

  /* Set the current position at the start of the list. */
  inline void moveToStart() override {
    CHECK_STATE();
    m_pos = 0;
  }

  /* Set the current position at the end of the list. */
  inline void moveToEnd() override {
    CHECK_STATE();
    if (m_length) {
      m_pos = m_length - 1;
    }
  }

  /* Move the current position one step left unless already at the beginning. */
  inline void prev() override {
    CHECK_STATE();
    if (m_pos > 0) {
      m_pos--;
    }
  }

  /* Move the current position one step right unless already at the end. */
  inline void next() override {
    CHECK_STATE();
    if (m_length > 0 && m_pos < m_length - 1) {
      m_pos++;
    }
  }

  /* Return the number of items in the list. */
  inline size_t length() const override {
    CHECK_STATE();
    return m_length;
  }

  /* Return the current position of the list. */
  inline size_t currPos() const override {
    CHECK_STATE();
    if (m_length) {
      assert(m_pos < m_length);
    }
    return m_pos;
  }

  /* Set current position. */
  inline void moveToPos(size_t pos) override {
    CHECK_STATE();
    if (pos >= m_length) {
      throw std::runtime_error(
          "Attempt to move to position beyond the length of the list.");
    }
    m_pos = pos;
  }

  /* Return the value of the current element */
  inline T getValue() const override {
    CHECK_STATE();
    if (m_length == 0) {
      throw std::runtime_error("Attempting to access element from empty list.");
    }
    return m_data[m_pos];
  }

  /* Search for an item in the list, returns npos ( (size_t) -1 ) if not found.
   */
  size_t Search(const T &item) const override {
    for (size_t i = 0; i < m_length; i++) {
      if (m_data[i] == item) {
        return i;
      }
    }
    return npos;
  }
#undef CHECK_STATE
};

} // namespace cse204
