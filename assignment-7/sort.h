#include <array>
#include <cassert>
#include <cctype>
#include <concepts>
#include <iostream>
#include <random>
#include <type_traits>

namespace cse204 {

using std::size_t;
constexpr unsigned seed = 7;

inline size_t random() { 
  return std::rand(); 
}

template <class R>
concept ArrayLike = requires(R a) {
  a[0];
  std::size(a);
};

void insertion_sort(ArrayLike auto &A, size_t l, size_t r) {
  for (size_t i = l + 1; i < r; i++) {
    auto v = A[i];
    size_t j = i;
    while (j > l && A[j - 1] > v) {
      A[j] = A[j - 1];
      j--;
    }
    A[j] = v;
  }
}

void insertion_sort(ArrayLike auto &A) { insertion_sort(A, 0, std::size(A)); }

void merge(ArrayLike auto &A, auto *temp, size_t l, size_t l1, size_t r1, size_t l2, size_t r2) {
  for (size_t i = l1; i < r1; i++) {
    temp[i] = A[i];
  }
  for (size_t i = l2; i < r2; i++) {
    temp[i] = A[i];
  }
  while (l1 < r1 && l2 < r2) {
    if (temp[l1] < temp[l2]) {
      A[l++] = temp[l1++];
    } else {
      A[l++] = temp[l2++];
    }
  }
  while (l1 < r1) {
    A[l++] = temp[l1++];
  }
  while (l2 < r2) {
    A[l++] = temp[l2++];
  }
}

void merge_sort_impl(ArrayLike auto &A, auto *temp, size_t l, size_t r) {
  if (r - l <= 1)
    return;
  auto mid = (l + r) / 2;
  merge_sort_impl(A, temp, l, mid);
  merge_sort_impl(A, temp, mid, r);
  merge(A, temp, l, l, mid, mid, r);
}

void merge_sort(ArrayLike auto &A) {
  using T = std::remove_reference_t<decltype(A[0])>;
  auto n = std::size(A);
  T *temp = new T[n];
  merge_sort_impl(A, temp, 0, n);
  delete[] temp;
}

size_t lomuto_partition(ArrayLike auto &A, size_t l, size_t r) {
  auto pivot = A[r - 1];
  auto p = l;
  for (auto i = l; i + 1 < r; i++) {
    if (A[i] <= pivot) {
      std::swap(A[p++], A[i]);
    }
  }
  std::swap(A[p], A[r - 1]);
  return p;
}

size_t hoare_partition(ArrayLike auto &A, size_t l, size_t r) {
  auto pivot = A[(l + r) / 2];
  auto p = l, q = r - 1;
  while (true) {
    while (A[p] < pivot)
      p++;
    while (A[q] > pivot)
      q--;
    if (p < q)
      std::swap(A[p++], A[q--]);
    else
      return p;
  }
}

size_t lomuto_partition_random(ArrayLike auto &A, size_t l, size_t r) {
  std::swap(A[r - 1], A[l + random() % (r - l)]);
  return lomuto_partition(A, l, r);
}

size_t hoare_partition_random(ArrayLike auto &A, size_t l, size_t r) {
  std::swap(A[(l + r) / 2], A[l + random() % (r - l)]);
  return lomuto_partition(A, l, r);
}

enum class PartitionScheme { LOMUTO, HOARE, LOMUTO_RANDOM, HOARE_RANDOM };

template <PartitionScheme scheme> void quick_sort_impl(ArrayLike auto &A, size_t l, size_t r) {
  if (r - l <= 1)
    return;
  int incr = 1;
  size_t p;
  if constexpr (scheme == PartitionScheme::LOMUTO)
    p = lomuto_partition(A, l, r);
  else if constexpr (scheme == PartitionScheme::HOARE)
    p = hoare_partition(A, l, r), incr = 0;
  else if constexpr (scheme == PartitionScheme::LOMUTO_RANDOM)
    p = lomuto_partition_random(A, l, r);
  else if constexpr (scheme == PartitionScheme::HOARE_RANDOM)
    p = hoare_partition_random(A, l, r), incr = 0;
  quick_sort_impl<scheme>(A, l, p);
  quick_sort_impl<scheme>(A, p + incr, r);
}

template <PartitionScheme scheme = PartitionScheme::LOMUTO> void quick_sort(ArrayLike auto &A) {
  quick_sort_impl<scheme>(A, 0, std::size(A));
}

void quick_sort_random(ArrayLike auto &A) { quick_sort_impl<PartitionScheme::LOMUTO_RANDOM>(A, 0, std::size(A)); }

} // namespace cse204
