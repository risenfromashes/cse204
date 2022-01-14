#include <cassert>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <memory>
#include <pthread.h>
#include <utility>
#include <vector>

class Heap {

  using size_t = uint32_t;

  std::allocator<int> m_allocator;

  size_t m_capacity;
  size_t m_size;
  int *m_array;

  bool m_owns_memory = true;

  inline static constexpr size_t k_default_size = 8;

  inline size_t parent(size_t i) { return i >> 1; }
  inline size_t left(size_t i) { return i << 1; }
  inline size_t right(size_t i) { return left(i) | 1; }

  inline void heapify(size_t p) {
    size_t l, r, lr;
    while (p) {
      l = left(p);
      r = right(p);

      lr = p;
      if (l <= m_size && m_array[l] > m_array[lr]) {
        lr = l;
      }
      if (r <= m_size && m_array[r] > m_array[lr]) {
        lr = r;
      }

      if (lr != p) {
        std::swap(m_array[p], m_array[lr]);
        p = lr;
      } else {
        break;
      }
    }
  }

  inline bool is_heap(size_t p) {
    if (p > m_size) {
      return true;
    }
    size_t l = left(p);
    size_t r = right(p);

    if ((l <= m_size) && (m_array[l] > m_array[p])) {
      return false;
    }
    if (r <= m_size && m_array[r] > m_array[p]) {
      return false;
    }
    return is_heap(left(p)) && is_heap(right(p));
  }

  inline void build_heap() {
    for (int i = (m_size / 2); i >= 1; i--) {
      heapify(i);
    }
  }

  void expand() {
    if (!m_owns_memory) {
      throw std::runtime_error("Heap size overflow");
    }
    assert(m_size <= m_capacity);

    size_t new_capacity =
        m_capacity < k_default_size ? k_default_size : m_capacity * 2;
    auto *new_arr = m_allocator.allocate(new_capacity + 1);

    std::uninitialized_copy(m_array + 1, m_array + m_size + 1, new_arr + 1);

    m_allocator.deallocate(m_array, m_capacity + 1);
    m_array = new_arr, m_capacity = new_capacity;
  }

  inline void increase_key(size_t k, int v) {
    size_t p, q = k;
    while (q > 1 && m_array[p = parent(q)] < v) {
      m_array[q] = m_array[p];
      q = p;
    }
    m_array[q] = v;
  }

  inline void update_key(size_t k, int v) {
    if (v > m_array[k]) {
      increase_key(k, v);
    } else {
      m_array[k] = v;
      heapify(k);
    }
  }

public:
  inline void insert(int v) {
    if (m_size >= m_capacity) [[unlikely]] {
      expand();
    }
    increase_key(++m_size, v);
  }

  inline int extract_max() {
    if (m_size < 1) {
      throw std::runtime_error("Trying to remove from empty heap");
    }

    int v = m_array[1];
    m_array[1] = m_array[m_size];
    heapify(1);
    m_size--;
    return v;
  }

  inline int deleteKey() { return extract_max(); }

  inline size_t size() { return m_size; }

  inline int top() { return m_array[1]; }

  inline int getMax() { return m_array[1]; }

  Heap(size_t initial_capacity = k_default_size)
      : m_capacity(initial_capacity), m_size(0),
        m_array(m_allocator.allocate(m_capacity + 1)) {}

  Heap(std::vector<int> &vec)
      : m_capacity(vec.capacity() - 1), m_size(vec.size()),
        m_array(vec.data() - 1), m_owns_memory(false) {
    build_heap();
  }
  ~Heap() {
    if (m_owns_memory) {
      m_allocator.deallocate(m_array, m_capacity + 1);
    }
  }

  void print(std::ostream &os = std::cout) {
    int n = size();
    int w = 8;
    int m = 2;
    int H = std::floor(std::log2(n));
    int r = 1 << H;
    int i = 1, d = 0, h = H;
    do {
      int s = ((1 << h) - 1) * ((w + m) / 2);
      for (int k = 0; k < s; k++) {
        os << ' ';
      }
      while (i < (1 << (d + 1))) {
        os << std::setw(w) << m_array[i] << std::setw(1);
        for (int k = 0; k < (2 * s + m); k++) {
          os << ' ';
        }
        i++;
        if (i > m_size) {
          std::cout << std::endl;
          break;
        }
      }
      std::cout << std::endl;
      ++d;
      --h;
    } while (d <= H);
  }
};

inline void heapsort(std::vector<int> &vec) {
  Heap heap(vec);
  for (int i = vec.size() - 1; i >= 1; i--) {
    vec[i] = heap.deleteKey();
  }
  std::reverse(vec.begin(), vec.end());
}
