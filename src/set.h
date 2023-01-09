#pragma once

#include "binary_tree.h"
#include "vector.h"

namespace containers {
template <class Key>
class set : public containers::BinaryTree<Key> {
 public:
  using key_type = Key;
  using value_type = Key;

  using iterator = typename containers::BinaryTree<value_type>::iterator;
  using const_iterator =
      typename containers::BinaryTree<value_type>::const_iterator;
  using node = typename containers::BinaryTree<value_type>::Node;

  set() : containers::BinaryTree<value_type>::BinaryTree() {}
  explicit set(std::initializer_list<value_type> const& items)
      : containers::BinaryTree<value_type>::BinaryTree() {
    typename std::initializer_list<value_type>::const_iterator i =
        items.begin();
    while (i != items.end()) {
      if (!(this->contains(*i))) this->insert(*i);
      ++i;
    }
  }
  set(const set& s) : containers::BinaryTree<value_type>::BinaryTree(s) {}
  set(set&& s) : containers::BinaryTree<value_type>::BinaryTree(std::move(s)) {}
  ~set() {}

  void merge(set& other) {
    if (this->empty()) {
      this->swap(other);
    } else {
      iterator i = other.begin();
      while (i != other.end()) {
        if (!(this->contains(*i))) {
          this->merge_node(i++, other);
        } else {
          ++i;
        }
      }
    }
  }

  template <class... Args>
  containers::vector<std::pair<iterator, bool>> emplace(Args&&... args) {
    containers::vector<std::pair<iterator, bool>> v;
    const value_type data[] = {args...};
    for (const value_type item : data) {
      v.push_back(this->insert(item));
    }
    return v;
  }
};
}  // namespace containers
