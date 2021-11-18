#pragma once

#include <cassert>

#include "list.h"

namespace cse204 {

template <typename T> class linkedlist : public list<T> {

  using typename list<T>::size_t;
  using list<T>::npos;

  struct node {
    T item;
    node *next;

    /* Constructor to create sentinel node */
    node(std::nullptr_t) : item{}, next(nullptr) {}
    /* Constructor to create node with the item to contain */
    node(const T &item, node *next = nullptr) : item(item), next(next) {}
  };

  /* Sentinel node, to indicate the start of the list. First actual node is
   * m_head->next. */
  node *m_head;
  /* last node. m_tail == nullptr indicates list is empty */
  node *m_tail;
  /* node before current node, sentinel node if at the start */
  node *m_curr;

  size_t m_length;
  size_t m_pos;

public:
  /* Creates empty list. */
  linkedlist()
      : m_head(new node(nullptr)), m_tail(nullptr), m_curr(m_head), m_length(0),
        m_pos(0) {}

  /* List interface implementation. */

  /* Clear contents from the list, making it empty */
  void clear() override {
    node *p = m_head->next;
    while (p) {
      node *t = p;
      p = p->next;
      delete t;
    }
    // reset everything
    m_head->next = m_tail = nullptr;
    // set position at the start
    m_curr = m_head;
    // reset size and position
    m_pos = m_length = 0;
  }

  /* Insert an item at the current location of the list. */
  void insert(const T &item) override {
    assert(m_curr);

    node *new_node = new node(item);

    new_node->next = m_curr->next;
    m_curr->next = new_node;

    if (!m_tail) {
      // list was empty
      assert(!m_head->next);
      assert(m_head == m_curr);
      assert(m_length == 0 && m_pos == 0);

      m_tail = new_node;
    } else if (m_curr == m_tail) {
      // at the end of list
      assert(m_pos == m_length);
      assert(!new_node->next);
      // inserted node is now the tail of the list
      m_tail = new_node;
    }
    m_length++;
  }

  /* Append an item at the end of list. */
  void append(const T &item) override {
    // insert after setting position at the end
    node *curr_node = m_curr;
    m_curr = m_tail;
    insert(item);
    m_curr = curr_node;
  }

  /* Remove the current item and return it. */
  T remove(const T &item) override {
    assert(m_curr);
    // list is empty if m_tail is null
    if (!m_tail) {
      assert(m_curr == m_head);
      assert(m_length == 0);
      throw std::runtime_error("Attempt to remove element from empty list.");
    }

    // if list is already at the end, nothing to remove
    if (m_curr == m_tail) {
      assert(!m_curr->next);
      assert(m_pos == m_length);
      throw std::runtime_error(
          "Attempt to remove element from after the end of the list.");
    }

    // there must be a node after m_curr at this point
    assert(m_curr->next);

    /* The node to remove is the node after m_curr. */
    node *del_node = m_curr->next;
    T del_val = del_node->item;

    m_curr->next = del_node->next;

    delete del_node;

    m_length--;
    return del_val;
  }

  /* Set the current position to the start of the list. */
  void moveToStart() override {
    // m_head sentinel node indicates position is before head
    m_curr = m_head;
    m_pos = 0;
  }

  /* Set the current position to the end of the list. */
  void moveToEnd() override {
    m_curr = m_tail;
    m_pos = m_length;
  }

  /* Move the current position one step left, unless already at the beginning.
   */
  void prev() override {
    assert(m_curr);

    if (m_curr == m_head) {
      // nothing to do if already at the beginning
      assert(m_pos == 0);
      return;
    }
    // list must not be empty if m_curr is not at the beginning
    assert(m_tail && m_length > 0);

    // since its a singly linked list, have to iterate from start to get to
    // previous node
    node *p = m_head;
    while (p->next != m_curr) {
      p = p->next;
    }
    m_curr = p;

    m_pos--;
  }

  /* Move the current position one step right, unless already at the end */
  void next() override {
    assert(m_curr);

    if (m_curr == m_tail) {
      // already at the end, nothing to do
      assert(m_tail->next == nullptr);
      assert(m_pos == m_length);
      return;
    }

    m_curr = m_curr->next;
    m_pos++;
  }

  /* Return the number of elements in the list. */
  size_t length() const override { return m_length; }

  /* Return the position of the current element. */
  size_t currPos() const override { return m_pos; }

  /* Set current position. */
  void moveToPos(size_t pos) override {
    if (pos > m_length) {
      throw std::runtime_error(
          "Attempt to move to position beyond the length of the list.");
    }

    for (m_pos = 0, m_curr = m_head; m_pos < pos;
         m_pos++, m_curr = m_curr->next) {
      assert(m_curr);
    }
  }

  /* Return the current element. */
  T getValue() const override {
    assert(m_curr);
    // list is empty if m_tail is null
    if (!m_tail) {
      assert(m_curr == m_head);
      assert(m_length == 0);
      throw std::runtime_error(
          "Attempt to get value of element from empty list.");
    }

    // if list is already at the end, nothing to remove
    if (m_curr == m_tail) {
      assert(!m_curr->next);
      assert(m_pos == m_length);
      throw std::runtime_error(
          "Attempt to get value of element from after the end of the list.");
    }

    // list must contain node after m_curr at this point
    assert(m_curr->next);

    return m_curr->next->item;
  }

  /* Search for an item in the list, returns npos ( (size_t) -1 ) if not found.
   */
  size_t search(const T &item) const override {
    node *p = m_head->next;
    size_t i = 0;
    while (i < m_length) {
      p = p->next;
      i++;
      if (p->item == item) {
        return i;
      }
    }
    return npos;
  }
};
} // namespace cse204
