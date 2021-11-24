#pragma once

#include <cassert>
#include <cstddef>
#include <ostream>
#include <sstream>

namespace cse204 {

/* Abstract list interface */
template <typename T> class list {
protected:
  typedef std::size_t size_t;
  static const size_t npos = (size_t)-1;

public:
  /* List ADT methods */
  virtual void clear() = 0;
  virtual void insert(const T &item) = 0;
  virtual void append(const T &item) = 0;
  virtual T remove() = 0;
  virtual void moveToStart() = 0;
  virtual void moveToEnd() = 0;
  virtual void prev() = 0;
  virtual void next() = 0;
  virtual size_t length() const = 0;
  virtual size_t currPos() const = 0;
  virtual void moveToPos(size_t pos) = 0;
  virtual T getValue() const = 0;
  virtual size_t Search(const T &item) const = 0;
};

/* Print a list to output stream */
template <typename T>
std::ostream &operator<<(std::ostream &os, list<T> &list) {
  // remember current position
  os << '<';
  if (list.length() > 0) {

    size_t prev_pos = list.currPos();
    for (list.moveToStart();; list.next()) {
      if (prev_pos == list.currPos()) {
        os << "| ";
      }
      os << list.getValue();
      if (list.currPos() == list.length() - 1) {
        break;
      } else {
        os << ' ';
      }
    }
    list.moveToPos(prev_pos);
  }
  os << '>';
  // restore current position
  return os;
}

/* Print a list to a string */
template <typename T> std::string to_string(list<T> &list) {
  std::ostringstream oss;
  oss << list;
  return oss.str();
}

/* Check element-wise equality between lists */
template <typename T> bool operator==(list<T> &a, list<T> &b) {
  if (a.length() != b.length()) {
    return false;
  }
  size_t pos_a = a.currPos(), pos_b = b.currPos();
  for (a.moveToStart(), b.moveToStart();; a.next(), b.next()) {
    if (a.getValue() != b.getValue()) {
      return false;
    }
  }
  // Restore postion
  a.moveToPos(pos_a);
  b.moveToPos(pos_b);
  return true;
}
} // namespace cse204
