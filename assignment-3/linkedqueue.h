#pragma once

#include <algorithm>
#include <cassert>

#include "queue.h"

namespace cse204 {

template <class T, class Allocator = std::allocator<T>>
class LinkedQueue : public Queue<T, Allocator> {
public:
  struct node {
    T item;
    node *next;
    /* Constructor to create sentinel node */
    node(node *next) : next(next), item {}
    {}
    /* Constructor to create node with the item to contain */
    template <class... R>
    requires std::constructible_from<T, R...> //
    node(node *next, R &&...params)
        : next(next), item(std::forward<R>(params)...) {}
  };

protected:
  using size_t = typename Queue<T, Allocator>::size_t;
  using allocator_type =
      typename std::allocator_traits<Allocator>::template rebind_alloc<node>;
  using allocator_traits =
      typename std::allocator_traits<Allocator>::template rebind_traits<node>;

  allocator_type m_allocator;

  /* Sentinel node, to indicate the start of the queue. First actual node is
   * m_head->next. */
  node *m_head;
  /* Last node; m_head if empty */
  node *m_tail;

  size_t m_length;

public:
  /* Create empty queue. */
  LinkedQueue() : m_length(0) {
    m_head = m_tail = allocator_traits::allocate(m_allocator, 1);
    allocator_traits::construct(m_allocator, m_head, nullptr);
  }

  /* Create queue from initialiser list */
  LinkedQueue(std::initializer_list<T> items) : LinkedQueue() {
    for (const T &item : items) {
      enqueue(item);
    }
  }

  /* Copy construct: copies elements from another queue */
  LinkedQueue(const LinkedQueue &other) requires std::copyable<T>
      : m_length(0) {
    m_head = m_tail = allocator_traits::allocate(m_allocator, 1);
    allocator_traits::construct(m_allocator, m_head, nullptr);
    copy_from(other);
  }

  /* Move constructor: steals elements from another queue */
  LinkedQueue(LinkedQueue &&other)
      : m_head(other.m_head), m_tail(other.m_tail), m_length(other.m_length) {
    /* Reset moved from linkedqueue to initial empty queue */
    other.m_length = 0;
    other.m_head = other.m_tail = allocator_traits::allocate(m_allocator, 1);
    allocator_traits::construct(m_allocator, other.m_head, nullptr);
  }

  /* Copy assignment: copy elements from another queue */
  LinkedQueue &operator=(const LinkedQueue &other) requires std::copyable<T> {
    if (this == &other) {
      // to handle self-assignment
      return *this;
    }
    delete_elements();
    copy_from(other);
    return *this;
  }

  /* Move assignment: steal elements from another queue */
  LinkedQueue &operator=(LinkedQueue &&other) {
    if (this == &other) {
      // to handle self-assignment
      return *this;
    }
    // delete current elements and steal everything from other queue
    delete_elements();
    // swap the head sentinel node and tail nodes as well
    std::swap(m_head, other.m_head);
    std::swap(m_tail, other.m_tail);
    m_length = other.m_length;
    // reset other queue
    other.m_head->next = nullptr;
    other.m_tail = other.m_head;
    other.m_length = 0;

    return *this;
  }

  /* Destructor */
  ~LinkedQueue() {
    delete_elements();
    allocator_traits::destroy(m_allocator, m_head);
    allocator_traits::deallocate(m_allocator, m_head, 1);
  }

private:
  /* helper methods */

  /* delete all the nodes in the queue, except the head */
  void delete_elements() {
    node *p = m_head->next;
    while (p) {
      node *t = p;
      p = p->next;
      allocator_traits::destroy(m_allocator, t);
      allocator_traits::deallocate(m_allocator, t, 1);
    }
  }

  /* Assuming the queue is cleared and do not have dangling resources, copy
   * elements from other queue */
  void copy_from(const LinkedQueue<T> &other) {
    m_length = other.m_length;
    // assume the other queue may be empty at this point
    m_head->next = nullptr;
    m_tail = m_head;

    if (m_length == 0) {
      return;
    }

    node *old_node = other.m_head;
    node *new_node = m_head;

    assert(old_node->next && other.m_tail); // should not be empty

    while (old_node->next) {
      old_node = old_node->next;
      new_node = new_node->next =
          allocator_traits::allocate(m_allocator, 1, new_node);
      allocator_traits::construct(m_allocator, new_node, nullptr,
                                  old_node->item);
      // new_node is the counterpart of old_node at this point
      if (old_node->next == nullptr) {
        assert(old_node == other.m_tail);
        m_tail = new_node;
      }
    }
  }

  /* construct element in place in the queue */
  template <class... R>
  requires std::constructible_from<T, R...>
  void emplace(R &&...params) {
    assert(m_tail);
    m_tail = m_tail->next = allocator_traits::allocate(m_allocator, 1, m_tail);
    allocator_traits::construct(m_allocator, m_tail, nullptr,
                                std::forward<R>(params)...);
    m_length++;
  }

public:
  /* Queue interface implementation. */

  /* Clear contents from the queue, making it empty */
  void clear() override {
    delete_elements();
    // reset everything
    m_head->next = nullptr;
    m_tail = m_head;
    // reset size
    m_length = 0;
  }

  /* Enqueue a copy of item at the back of the queue */
  void enqueue(const T &item) override { emplace(item); }

  /* Enqueue by moving item at the back of the queue */
  void enqueue(T &&item) override { emplace(std::move(item)); }

  /* Dequeues an item from the front of the queue and returns value */
  T dequeue() override {
    if (m_length == 0) {
      throw std::runtime_error("Attempt to dequeue from empty queue");
    }
    assert(m_head->next && m_tail != m_head);
    node *front = m_head->next;
    // front might be the only node right now
    if (front == m_tail) {
      assert(m_length == 1);
      m_tail = m_head;
    }
    // move object if move constructor is available
    T ret = std::move(front->item);
    m_head->next = front->next;
    // delete node
    allocator_traits::destroy(m_allocator, front);
    allocator_traits::deallocate(m_allocator, front, 1);
    m_length--;
    return ret;
  }

  /* Returns size of the queue */
  inline size_t length() const override { return m_length; }

  /* Returns the value of the front element of the queue */
  inline T &frontValue() override {
    if (m_length == 0) {
      throw std::runtime_error("Attempt to get front value from empty queue");
    }
    assert(m_head->next);
    return m_head->next->item;
  }

  /* Returns the value of the front element of the queue */
  inline const T &frontValue() const override { return frontValue(); }

  /* Returns the value of the rear element of the queue */
  inline T &rearValue() override {
    if (m_length == 0) {
      throw std::runtime_error("Attempt to get rear value from empty queue");
    }
    assert(m_head != m_tail && m_tail);
    return m_tail->item;
  }

  /* Returns the value of the rear element of the queue */
  inline const T &rearValue() const override { return rearValue(); }

  /* Returns the value of the top element of the queue */
  T leaveQueue() override {
    if (m_length == 0) {
      throw std::runtime_error("Attempt to dequeue from empty queue");
    }

    assert(m_head->next && m_tail != m_head);
    // tail is the rear node to remove, and we need to make the node before rear
    // the new tail
    node *rear = m_tail;
    m_tail = m_head;
    while (m_tail->next != rear) {
      m_tail = m_tail->next;
    }
    // move object if move constructor is available
    T ret = std::move(rear->item);
    m_tail->next = nullptr;
    // delete node
    allocator_traits::destroy(m_allocator, rear);
    allocator_traits::deallocate(m_allocator, rear, 1);
    m_length--;
    return ret;
  }
};

} // namespace cse204
