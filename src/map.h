#pragma once

#include <stdexcept>

#include "binary_tree.h"
#include "vector.h"

namespace containers {
template <class Key, class T>
class map : public containers::BinaryTree<std::pair<const Key, T>> {
 public:
  using key_type = Key;
  using mapped_type = T;
  using value_type = std::pair<const key_type, mapped_type>;
  using reference = value_type&;
  using const_reference = const value_type&;
  using iterator = typename containers::BinaryTree<value_type>::iterator;
  using const_iterator =
      typename containers::BinaryTree<value_type>::const_iterator;
  using size_type = size_t;

  using pair = std::pair<const key_type, mapped_type>;
  using node = typename containers::BinaryTree<value_type>::Node;

  map() : containers::BinaryTree<value_type>::BinaryTree() {}
  explicit map(std::initializer_list<value_type> const& items)
      : containers::BinaryTree<value_type>::BinaryTree() {
    typename std::initializer_list<value_type>::const_iterator i =
        items.begin();
    while (i != items.end()) {
      if (!(contains(std::get<0>(*i)))) insert(*i);
      ++i;
    }
  }
  map(const map& m) : containers::BinaryTree<value_type>::BinaryTree() {
    iterator i = m.begin();
    while (i != m.end()) {
      insert(*i);
      ++i;
    }
  }
  map(map&& m) : containers::BinaryTree<value_type>::BinaryTree(std::move(m)) {}
  ~map() {}

  T& at(const Key& key) {
    node* n = find_node(key);
    if (n == nullptr)
      throw std::out_of_range("There's no obj in map with such key");
    return std::get<1>(n->key);
  }

  T& operator[](const Key& key) {
    node* n = find_node(key);
    if (n)
      return std::get<1>(n->key);
    else
      return std::get<1>(*(std::get<0>(insert(key, mapped_type()))));
  }

  std::pair<iterator, bool> insert(const value_type& value) {
    if (this->empty()) {
      this->insert_root(value);
      return std::pair<iterator, bool>{
          iterator(containers::BinaryTree<value_type>::root), true};
    }
    node* parent_node = containers::BinaryTree<value_type>::root;
    while ((parent_node->left || parent_node->right) ||
           std::get<0>(parent_node->key) == std::get<0>(value)) {
      if (std::get<0>(parent_node->key) > std::get<0>(value)) {
        if (parent_node->left)
          parent_node = parent_node->left;
        else
          break;
      } else {
        if (parent_node->right && iterator(parent_node->right) != this->end())
          parent_node = parent_node->right;
        else
          break;
      }
    }
    bool not_equal_keys = std::get<0>(parent_node->key) != std::get<0>(value);
    return std::pair<iterator, bool>{
        iterator(insert_node(value, not_equal_keys ? parent_node : nullptr)),
        not_equal_keys};
  }

  std::pair<iterator, bool> insert(const Key& key, const T& obj) {
    return insert(pair{key, obj});
  }

  std::pair<iterator, bool> insert_or_assign(const Key& key, const T& obj) {
    node* n = find_node(key);
    if (n) {
      iterator i(n);
      std::get<1>(*i) = obj;
      return std::pair<iterator, bool>{i, false};
    } else {
      return insert(key, obj);
    }
  }

  void merge(map& other) {
    if (this->empty()) {
      this->swap(other);
    } else {
      iterator i = other.begin();
      while (i != other.end()) {
        if (!(contains(std::get<0>(*i)))) {
          merge_node(i++, other);
        } else {
          ++i;
        }
      }
    }
  }

  bool contains(const Key& key) const { return find_node(key) != nullptr; }

  template <class... Args>
  containers::vector<std::pair<iterator, bool>> emplace(Args&&... args) {
    containers::vector<std::pair<iterator, bool>> v;
    const value_type data[] = {args...};
    for (const value_type item : data) {
      v.push_back(insert(item));
    }
    return v;
  }

 private:
  node* insert_node(value_type k, node* p) {
    if (p) {
      node* n = new node(k, p);
      if (std::get<0>(k) < std::get<0>(p->key)) {
        p->left = n;
      } else {
        if (p->right) {
          n->right = p->right;
          p->right->parent = n;
        }
        p->right = n;
      }
      return n;
    }
    return nullptr;
  }

  void merge_node(iterator i, map& other) {
    node* n = other.find_node(std::get<0>(*i));
    node* p = find_parent(n->key);
    other.remove_node(n);
    n->parent = p;
    n->left = nullptr;
    n->right = nullptr;
    if (std::get<0>(n->key) < std::get<0>(p->key)) {
      p->left = n;
    } else {
      if (p->right) {
        p->right->parent = n;
        n->right = p->right;
      }
      p->right = n;
    }
  }

  node* find_node(const Key& key) const {
    node* n = this->root;
    while (n && std::get<0>(n->key) != key) {
      if (key < std::get<0>(n->key)) {
        n = n->left;
      } else {
        n = n->right;
      }
    }
    return n;
  }

  node* find_parent(value_type key) const {
    node* p = this->root;
    while (p->left || (p->right && iterator(p->right) != this->end())) {
      if (std::get<0>(key) < std::get<0>(p->key) && p->left)
        p = p->left;
      else if (std::get<0>(key) > std::get<0>(p->key) && p->right)
        p = p->right;
      else
        break;
    }
    return p;
  }
};
}  // namespace containers
