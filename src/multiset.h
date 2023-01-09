#pragma once

#include "binary_tree.h"

namespace containers {
template <class Key>
class multiset : public containers::BinaryTree<Key> {
 public:
  using key_type = Key;
  using value_type = Key;
  using size_type = size_t;

  using iterator = typename containers::BinaryTree<key_type>::iterator;
  using const_iterator =
      typename containers::BinaryTree<key_type>::const_iterator;
  using node = typename containers::BinaryTree<value_type>::Node;

  multiset() : containers::BinaryTree<key_type>::BinaryTree() {}
  explicit multiset(std::initializer_list<value_type> const& items)
      : containers::BinaryTree<key_type>::BinaryTree() {
    typename std::initializer_list<value_type>::const_iterator i =
        items.begin();
    while (i != items.end()) {
      insert(*i);
      ++i;
    }
  }
  multiset(const multiset& s) : containers::BinaryTree<value_type>() {
    iterator i = s.begin();
    while (i != s.end()) {
      insert(*i);
      ++i;
    }
  }
  multiset(multiset&& s)
      : containers::BinaryTree<key_type>::BinaryTree(std::move(s)) {}
  ~multiset() {}

  iterator insert(const value_type& value) {
    if (this->empty()) {
      this->insert_root(value);
      return iterator(this->root);
    }
    node* parent_node = this->root;
    while (parent_node->left || parent_node->right) {
      if (parent_node->key > value) {
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
    return iterator(this->insert_node(value, parent_node));
  }

  void merge(BinaryTree<key_type>& other) {
    if (this->empty()) {
      this->swap(other);
    } else {
      iterator i = other.begin();
      while (i != other.end()) this->merge_node(i++, other);
    }
  }

  size_type count(const Key& key) {
    iterator i = this->find(key);
    size_type n = 0;
    while (i != this->end() && *i == key) {
      ++i;
      ++n;
    }
    return n;
  }

  std::pair<iterator, iterator> equal_range(const Key& key) {
    iterator i = this->find(key);
    iterator j = i;
    while (j != this->end() && *j == key) ++j;
    return std::pair<iterator, iterator>{i, j};
  }

  iterator lower_bound(const Key& key) { return this->find(key); }

  iterator upper_bound(const Key& key) { return std::get<1>(equal_range(key)); }

  template <class... Args>
  containers::vector<iterator> emplace(Args&&... args) {
    containers::vector<iterator> v;
    const value_type data[] = {args...};
    for (const value_type item : data) v.push_back(insert(item));
    return v;
  }
};
}  // namespace containers
