#pragma once

#include <algorithm>
#include <cassert>

#include "stack.h"

namespace cse204 {

template <typename T> class LinkedStack : public Stack<T> {

  using typename Stack<T>::size_t;

  struct node {
    T item;
    node *next;
    /* Constructor to create sentinel node */
    node(node *next) : item{}, next(next) {}
    /* Constructor to create node with the item to contain */
    node(const T &item, node *next = nullptr) : item(item), next(next) {}
    node(T &&item, node *next = nullptr) : item(std::move(item)), next(next) {}
  };

  /* Sentinel node, to indicate the start of the list. First actual node is
   * m_head->next. */
  node *m_head;

  size_t m_length;

public:
  /* Create empty list. */
  LinkedStack() : m_head(new node(nullptr)), m_length(0) {}

  /* Create list from initialiser list */
  LinkedStack(std::initializer_list<T> items) : LinkedStack() {
    for (const T &item : items) {
      push(item);
    }
  }

  /* Copy construct: copies elements from another list */
  LinkedStack(const LinkedStack &other) requires std::copyable<T>
      : m_head(new node(nullptr)) {
    static_assert(std::copyable<T>);
    copy_from(other);
  }

  /* Move constructor: steals elements from another list */
  LinkedStack(LinkedStack &&other)
      : m_head(other.m_head), m_length(other.m_length) {
    /* Reset moved from linkedstack to initial empty list */
    other.m_head = new node(nullptr);
    other.m_length = 0;
  }

  /* Copy assignment: copy elements from another list */
  LinkedStack &operator=(const LinkedStack &other) requires std::copyable<T> {
    if (this == &other) {
      // to handle self-assignment
      return *this;
    }
    delete_elements();
    copy_from(other);

    return *this;
  }

  /* Move assignment: steal elements from another list */
  LinkedStack &operator=(LinkedStack &&other) {
    if (this == &other) {
      // to handle self-assignment
      return *this;
    }
    // delete current elements and steal everything from other list
    delete_elements();
    // swap the head sentinel nodes as well
    std::swap(m_head, other.m_head);
    m_length = other.m_length;
    // reset other list
    other.m_head->next = nullptr;
    other.m_length = 0;

    return *this;
  }

  /* Destructor */
  ~LinkedStack() {
    delete_elements();
    delete m_head;
  }

private:
  /* helper methods */

  /* delete all the nodes in the list, except the head */
  void delete_elements() {
    node *p = m_head->next;
    while (p) {
      node *t = p;
      p = p->next;
      delete t;
    }
  }

  /* Assuming the stack is cleared and do not have dangling resources, copy
   * elements from other list */
  void copy_from(const LinkedStack<T> &other) {
    m_length = other.m_length;
    // assume the other list may be empty at this point
    m_head->next = nullptr;

    if (m_length == 0) {
      return;
    }

    node *old_node = other.m_head;
    node *new_node = m_head;

    while (old_node->next) {
      // new_node is the counterpart of old_node at this point
      old_node = old_node->next;
      new_node->next = new node(old_node->item);
      new_node = new_node->next;
    }
  }

public:
  /* Stack interface implementation. */

  /* Clear contents from the list, making it empty */
  void clear() override {
    delete_elements();
    // reset everything
    m_head->next = nullptr;
    // reset size
    m_length = 0;
  }

  /* Pushes item on the top of the stack */
  void push(const T &item) override {
    if constexpr (std::copyable<T>) {
      node *new_node = new node(item, m_head->next);
      assert(m_head);
      m_head->next = new_node;
      m_length++;
    }
  }

  /* Pushes item on the top of the stack */
  void push(T &&item) override {
    node *new_node = new node(std::move(item), m_head->next);
    assert(m_head);
    m_head->next = new_node;
    m_length++;
  }

  /* Pops item from the top of the stack and returns value */
  T pop() override {
    if (m_length == 0) {
      throw std::runtime_error("Attempt to pop from empty stack");
    }
    assert(m_head->next);
    node *next = m_head->next;
    /* move object if move constructor is available */
    T ret = [next]() constexpr {
      if constexpr (std::move_constructible<T>) {
        return std::move(next->item);
      } else {
        return next->item;
      }
    }
    ();
    m_head->next = next->next;
    delete next;
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
    assert(m_head->next);
    return m_head->next->item;
  }

  /* Returns the value of the top element of the stack */
  inline const T &topValue() const override { return topValue(); }

  /* Has no effect */
  void setDirection(int) override {}
};

#ifndef CSE204_STACK_OUTPUT_STREAM_IMPLEMENTED
#define CSE204_STACK_OUTPUT_STREAM_IMPLEMENTED
template <typename T>
std::ostream &operator<<(std::ostream &os, Stack<T> &stack) {
  LinkedStack<T> temp;
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
