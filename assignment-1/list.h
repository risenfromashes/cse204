#pragma once

#include <cstddef>
#include <ostream>

// abstract list interface
namespace cse204 {

template <typename T> class list {
protected:
  typedef std::size_t size_t;
  static const size_t npos = (size_t)-1;

public:
  // standard list methods
  virtual void clear() = 0;
  virtual void insert(const T &item) = 0;
  virtual void append(const T &item) = 0;
  virtual T remove(const T &item) = 0;
  virtual void moveToStart() = 0;
  virtual void moveToEnd() = 0;
  virtual void prev() = 0;
  virtual void next() = 0;
  virtual size_t length() const = 0;
  virtual size_t currPos() const = 0;
  virtual void moveToPos(size_t pos) = 0;
  virtual T getValue() const = 0;
  virtual size_t search(const T &item) const = 0;
};

// list print operator overload
template <typename T>
std::ostream &operator<<(std::ostream &os, list<T> &list) {
  // remember current position
  size_t prev_pos = list.currPos();
  for (list.moveToStart();; list.next()) {
    if (prev_pos == list.currPos()) {
      os << "| ";
    }
    os << list.getValue() << ' ';
    if (list.currentPos() == list.length() - 1) {
      break;
    }
  }
  // restore current position
  list.moveToPos(prev_pos);
  return os;
}

} // namespace cse204
