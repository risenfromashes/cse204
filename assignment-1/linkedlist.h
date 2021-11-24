#pragma once

#include <algorithm>
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
    node(node *next) : item{}, next(next) {}
    /* Constructor to create node with the item to contain */
    node(const T &item, node *next = nullptr) : item(item), next(next) {}
  };

  /* Sentinel node, to indicate the start of the list. First actual node is
   * m_head->next. */
  node *m_head;
  /* second to last node. m_tail == m_head indicates list is empty */
  node *m_tail;
  /* node before current node, m_head if list is empty */
  node *m_curr;

  size_t m_length;
  size_t m_pos;

public:
  /* Create empty list. */
  linkedlist()
      : m_head(new node(nullptr)), m_tail(m_head), m_curr(nullptr), m_length(0),
        m_pos(0) {}

  /* Create list from initialiser list */
  linkedlist(std::initializer_list<T> items) : linkedlist() {
    for (const T &item : items) {
      append(item);
    }
  }

  /* Createl ist with data from static array */
  template <size_t N> linkedlist(T const (&items)[N]) : linkedlist() {
    for (size_t i = 0; i < N; i++) {
      append(items[i]);
    }
  }

  /* creates list with pointer data */
  linkedlist(size_t num_items, T const *items) : linkedlist() {
    for (size_t i = 0; i < num_items; i++) {
      append(items[i]);
    }
  }

  /* Copy construct: copies elements from another list */
  linkedlist(const linkedlist<T> &other) : m_head(new node(nullptr)) {
    copy_from(other);
  }

  /* Move constructor: steals elements from another list */
  linkedlist(linkedlist<T> &&other)
      : m_head(new node(other.m_head->next)), m_tail(other.m_tail),
        m_curr(other.m_curr), m_length(other.m_length), m_pos(other.m_pos) {
    /* Reset moved from linkedlist to initial empty list */
    other.m_head->next = nullptr;
    other.m_curr = other.m_tail = other.m_head;
    m_length = m_pos = 0;
  }

  /* Copy assignment: copy elements from another list */
  linkedlist &operator=(const linkedlist &other) {
    if (this == &other) {
      // to handle self-assignment
      return *this;
    }
    delete_elements();
    copy_from(other);
    return *this;
  }

  /* Move assignment: steal elements from another list */
  linkedlist<T> &operator=(linkedlist<T> &&other) {
    if (this == &other) {
      // to handle self-assignment
      return *this;
    }
    // delete current elements and steal everything from other list
    delete_elements();
    m_head->next = other.m_head->next;
    m_tail = other.m_tail;
    m_curr = other.m_curr;
    m_length = other.m_length;
    m_pos = other.m_pos;
    // reset other list
    other.m_head->next = nullptr;
    other.m_curr = other.m_tail = m_head;
    m_length = m_pos = 0;
    return *this;
  }

  /* Destructor */
  ~linkedlist() {
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

  /* Assuming the list is cleared and do not have dangling resources, copy
   * elements from other list */
  void copy_from(const linkedlist<T> &other) {
    m_pos = 0;
    m_length = other.m_length;
    // assume the other list may be empty at this point
    m_head->next = nullptr;
    m_curr = m_tail = m_head;

    if (m_length == 0) {
      return;
    }

    node *old_node = other.m_head;
    node *new_node = m_head;

    while (old_node->next) {
      // increment m_pos until we create current element
      if (m_pos == other.m_pos) {
        m_curr = new_node;
      } else if (m_pos < other.m_pos) {
        m_pos++;
      }
      // new_node is the counterpart of old_node at this point
      if (old_node->next->next == nullptr) {
        assert(other.m_tail == old_node);
        m_tail = new_node;
      }

      new_node->next = new node(old_node->item);
      new_node = new_node->next;
    }
  }

  /* Run some assertions to check validity of state. */
  inline void check_state() {
    assert(m_curr && m_tail);
    if (m_length == 0) {
      assert(m_head->next == nullptr);
    }
    if (m_length <= 1) {
      assert(m_head == m_tail);
      assert(m_head == m_curr);
      assert(m_pos == 0);
    }
    if (m_length > 0) {
      assert(m_curr && m_curr->next);
      assert(m_head->next);
      assert(m_tail->next);
      assert(m_tail->next->next == nullptr);
    }
  }

public:
  /* List interface implementation. */

  /* Clear contents from the list, making it empty */
  void clear() override {
    delete_elements();
    // reset everything
    m_head->next = nullptr;
    m_curr = m_tail = m_head;
    // reset size and position
    m_pos = m_length = 0;
  }

  /* Insert an item at the current location of the list. */
  void insert(const T &item) override {
    check_state();

    node *new_node = new node(item);
    new_node->next = m_curr->next;
    m_curr->next = new_node;
    // if current element was the second most element, now the new node is
    if (m_tail == m_curr) {
      // if there is one element, tail can only be the same as head
      if (m_length > 0) {
        m_tail = new_node;
      }
    }
    m_length++;
  }

  /* Append an item at the end of list. */
  void append(const T &item) override {
    check_state();

    node *new_node = new node(item, nullptr);
    // if list is not empty, it will have one node after tail
    if (m_tail->next) {
      assert(m_length > 0);
      m_tail->next->next = new_node;
      m_tail = m_tail->next;
    } else {
      // if list is empty head, tail and curr wouldn't change
      assert(m_length == 0);
      m_head->next = new_node;
    }
    m_length++;
  }

  /* Remove the current item and return the value. */
  T remove() override {
    check_state();
    // list is empty if head->next is null
    if (m_head->next == nullptr) {
      assert(m_length == 0);
      throw std::runtime_error("Attempt to remove element from empty list.");
    }
    // there must be a node after m_curr at this point
    assert(m_curr->next);
    /* The node to remove is the node after m_curr. */
    node *del_node = m_curr->next;
    T del_val = del_node->item;

    m_curr->next = del_node->next;

    delete del_node;

    m_length--;

    if(m_curr == m_tail){
      prev();
      m_tail = m_curr;
    }
    return del_val;
  }

  /* Set the current position to the start of the list. */
  inline void moveToStart() override {
    check_state();
    // m_head sentinel node indicates position is before head
    m_curr = m_head;
    m_pos = 0;
  }

  /* Set the current position to the end of the list. */
  inline void moveToEnd() override {
    check_state();
    // second to last node indicates position is before the last node, which is
    // the END
    m_curr = m_tail;
    m_pos = m_length;
  }

  /* Move the current position one step left, unless already at the beginning.
   */
  void prev() override {
    check_state();

    if (m_curr == m_head) {
      // nothing to do if already at the beginning
      assert(m_pos == 0);
      return;
    }
    // list must not be empty if m_curr is not at the beginning
    assert(m_length > 0);

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
  inline void next() override {
    check_state();

    if (m_curr == m_tail) {
      // already at the end, nothing to do
      assert(m_pos == m_length - 1);
      return;
    }

    m_curr = m_curr->next;
    m_pos++;
  }

  /* Return the number of elements in the list. */
  inline size_t length() const override { return m_length; }

  /* Return the position of the current element. */
  inline size_t currPos() const override {
    assert(m_pos < m_length);
    return m_pos;
  }

  /* Set current position. */
  void moveToPos(size_t pos) override {
    check_state();

    if (pos >= m_length) {
      throw std::runtime_error(
          "Attempt to move to position beyond the length of the list.");
    }

    for (m_pos = 0, m_curr = m_head; m_pos < pos;
         m_pos++, m_curr = m_curr->next) {
      assert(m_curr);
    }
  }

  /* Return the current element. */
  inline T getValue() const override {
    check_state();
    // list is empty if no node follows head
    if (m_head->next == nullptr) {
      assert(m_length == 0);
      throw std::runtime_error(
          "Attempt to get value of element from empty list.");
    }
    // list must contain node after m_curr at this point
    assert(m_curr->next);
    return m_curr->next->item;
  }

  /* Search for an item in the list, returns npos ( (size_t) -1 ) if not found.
   */
  size_t Search(const T &item) const override {
    node *p = m_head;
    size_t i = 0;
    while (i < m_length) {
      p = p->next;
      assert(p);
      if (p->item == item) {
        return i;
      }
      i++;
    }
    return npos;
  }
};
} // namespace cse204
