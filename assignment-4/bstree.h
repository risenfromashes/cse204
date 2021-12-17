#include <cassert>
#include <concepts>
#include <functional>
#include <memory>

#include <iostream>

namespace cse204 {

enum class TraversalOrder { PRE_ORDER, POST_ORDER, IN_ORDER };
// empty value type
struct empty_t {};

template <std::totally_ordered K, class V = empty_t,
          class Allocator = std::allocator<V>>
class BSTree {

protected:
  struct Node {
    const K key;
    V value;

    Node *left = nullptr;
    Node *right = nullptr;

    template <class T>
    Node(T &&k) requires std::same_as<V, empty_t> : key{std::forward<T>(k)} {}

    template <class T, class R>
    Node(T &&k, R &&v) requires(!std::same_as<V, empty_t>)
        : key{std::forward<T>(k)}, value{std::forward<R>(v)} {}
  };

  using size_t = typename std::allocator_traits<Allocator>::size_type;
  using allocator_type =
      typename std::allocator_traits<Allocator>::template rebind_alloc<Node>;
  using allocator_traits =
      typename std::allocator_traits<Allocator>::template rebind_traits<Node>;

private:
  allocator_type m_allocator;

  Node *m_root;
  size_t m_size;

  /* helper methods */

  /* Get maximum element according to order relation in the tree */
  Node *find_max(Node *rt) const {
    if (!rt) {
      return nullptr;
    }
    auto *n = rt;
    while (n->right) {
      n = n->right;
    }
    return n;
  }

  /* Get maximum element according to order relation in the tree */
  Node *find_min(Node *rt) const {
    if (!rt) {
      return nullptr;
    }
    auto *n = rt;
    while (n->left) {
      n = n->left;
    }
    return n;
  }

  /* Get immediate previous item according to the order relation */
  Node *prev(Node *node) const { return find_max(node->left); }

  Node *next(Node *node) const { return find_min(node->right); }

  /* Find and return node containing key, return the parent of the empty leave
   * where the node would be otherwise */
  Node *find_node_impl(const K &key) const {
    if (!m_root) {
      return nullptr;
    }

    auto *n = m_root;
    while (true) {
      if (key == n->key) {
        return n;
      } else if (key < n->key && n->left) {
        n = n->left;
      } else if (key > n->key && n->right) {
        n = n->right;
      } else {
        break;
      }
    }
    return n;
  }

  /* insert new node into the tree */
  void insert_impl(Node *node) {
    // find prospective parent
    auto *p = find_node_impl(node->key);
    if (p) {
      if (node->key < p->key) {
        p->left = node;
      } else {
        p->right = node;
      }
    } else {
      // tree is currently empty
      assert(m_size == 0);
      m_root = node;
    }
  }

  /* return node to remove with given key */
  Node *remove_impl(const K &key, Node *&curr) {
    if (!curr) {
      throw std::runtime_error(
          "Attempting to remove element that doesn't exist");
    } else if (key < curr->key) {
      return remove_impl(key, curr->left);
    } else if (key > curr->key) {
      return remove_impl(key, curr->right);
    }

    Node *d = curr;
    if (curr->left && curr->right) { // if both children exist
      // bring immediate previous node at the root of current subtree
      Node *n = find_max(curr->left);
      // delete from original position
      assert(remove_impl(n->key, curr->left) == n);
      n->left = curr->left;
      n->right = curr->right;
      curr = n;
    } else if (curr->left) {
      curr = curr->left;
    } else {
      curr = curr->right;
    }
    return d;
  }

  void foreach_impl(const std::function<void(Node *)> &callable,
                    TraversalOrder order, Node *root) const {
    if (!root) {
      return;
    }

    switch (order) {
    case TraversalOrder::PRE_ORDER:
      callable(root);
      foreach_impl(callable, order, root->left);
      foreach_impl(callable, order, root->right);
      break;
    case TraversalOrder::POST_ORDER:
      foreach_impl(callable, order, root->left);
      foreach_impl(callable, order, root->right);
      callable(root);
      break;
    case TraversalOrder::IN_ORDER:
      foreach_impl(callable, order, root->left);
      callable(root);
      foreach_impl(callable, order, root->right);
      break;
    }
  }

  void print_tree_impl(std::ostream &os, Node *root) const {
    if (!root) {
      return;
    }
    os << root->key;
    if (root->left || root->right) {
      os << '(';
      if (root->left) {
        print_tree_impl(os, root->left);
      }
      os << ')' << '(';
      if (root->right) {
        print_tree_impl(os, root->right);
      }
      os << ')';
    }
  }

  void delete_tree(Node *node) {
    if (node) {
      delete_tree(node->left);
      delete_tree(node->right);
      allocator_traits::destroy(m_allocator, node);
      allocator_traits::deallocate(m_allocator, node, 1);
    }
  }

public:
  BSTree() : m_root(nullptr), m_size(0) {}
  ~BSTree() { delete_tree(m_root); }
  /* Insert overloads */

  /* Insert new key into the tree when there is no value type */
  void insert(const K &key) requires std::same_as<V, empty_t> {
    auto *new_node = allocator_traits::allocate(m_allocator, 1);
    allocator_traits::construct(m_allocator, new_node, key);
    insert_impl(new_node);
  }

  /* Insert new key into the tree when there is no value type */
  template <class R = K>
  requires std::constructible_from<K, R>
  void insert(R &&key) requires std::same_as<V, empty_t> {
    auto *new_node = allocator_traits::allocate(m_allocator, 1);
    allocator_traits::construct(m_allocator, new_node, std::forward<R>(key));
    insert_impl(new_node);
  }

  /* Insert new element */
  void insert(const K &key,
              const V &value) requires(!std::same_as<V, empty_t>) {
    auto *new_node = allocator_traits::allocate(m_allocator, 1);
    allocator_traits::construct(m_allocator, new_node, key, value);
    insert_impl(new_node);
  }

  /* Insert new element*/
  template <class T = K, class R = V>
  requires std::constructible_from<K, T> && std::constructible_from<V, R>
  void insert(T &&key, R &&value) requires(!std::same_as<V, empty_t>) {
    auto *new_node = allocator_traits::allocate(m_allocator, 1);
    allocator_traits::construct(m_allocator, new_node, std::forward<T>(key),
                                std::forward<R>(value));
    insert_impl(new_node);
  }

  /* Remove item with given key */
  auto remove(const K &key) {
    if (!m_root) {
      throw std::runtime_error("Attempt to remove from empty tree");
    }
    auto *d = remove_impl(key, m_root);

    auto r = [d]() constexpr {
      if constexpr (std::same_as<V, empty_t>) {
        return std::move(d->key);
      } else {
        return std::move(d->value);
      }
    }
    ();

    allocator_traits::destroy(m_allocator, d);
    allocator_traits::deallocate(m_allocator, d, 1);

    return r;
  }

  bool find(const K &key) const {
    return m_root && find_node_impl(key)->key == key;
  }

  BSTree &foreach (std::function<void(K)> &&callable,
                   TraversalOrder order) requires std::same_as<V, empty_t> {
    foreach_impl([&callable](Node *node) { callable(node->key); }, order,
                 m_root);
    return *this;
  }

  const BSTree &foreach (
      std::function<void(K)> &&callable,
      TraversalOrder order) const requires std::same_as<V, empty_t> {
    return foreach (*this, order);
  }

  BSTree &foreach (std::function<void(K, V)> &&callable,
                   TraversalOrder order) requires(!std::same_as<V, empty_t>) {
    foreach_impl([&callable](Node *node) { callable(node->key, node->value); },
                 order, m_root);
    return *this;
  }

  const BSTree &foreach (std::function<void(K, V)> &&callable,
                         TraversalOrder order) const
      requires(!std::same_as<V, empty_t>) {
    return foreach (*this, order);
  }

  void printTree(std::ostream &os) const { print_tree_impl(os, m_root); }

  friend std::ostream &operator<<(std::ostream &os, const BSTree<K, V> &tree) {
    tree.printTree(os);
    return os;
  }
};

} // namespace cse204
