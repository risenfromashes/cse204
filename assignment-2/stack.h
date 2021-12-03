#pragma once

#include <cassert>
#include <cstddef>
#include <ostream>
#include <concepts>
#include <sstream>

namespace cse204 {

/* Abstract list interface */
template <typename T> class Stack {
protected:
  typedef long long size_t;

public:
  /* List ADT methods */
  virtual void clear() = 0;
  virtual void push(const T &item) = 0;
  virtual void push(T &&item) = 0;
  virtual T pop() = 0;
  virtual size_t length() const = 0;
  virtual T &topValue() = 0;
  virtual const T &topValue() const = 0;
  virtual void setDirection(int direction) = 0;
};

template <typename T>
std::ostream &operator<<(std::ostream &os, Stack<T> &stack);

template <typename T>
std::string to_string(Stack<T> &stack){
  std::ostringstream oss;
  oss << stack;
  return oss.str();
}

} // namespace cse204
