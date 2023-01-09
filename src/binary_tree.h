#pragma once

#include <cstdint>
#include <limits>

namespace containers {
template <class T>
class BinaryTree {
 public:
  using value_type = T;
  using reference = value_type&;
  using const_reference = const value_type&;
  using size_type = size_t;

  struct Node {
    value_type key;
    Node* parent;
    Node* left;
    Node* right;

    Node()
        : key(value_type()), parent(nullptr), left(nullptr), right(nullptr) {}
    explicit Node(value_type k, Node* p = nullptr)
        : key(value_type(k)), parent(p), left(nullptr), right(nullptr) {}
  };

  class iterator {
    friend class BinaryTree;

   public:
    explicit iterator(Node* pointer = nullptr) : pointer_(pointer) {}

    reference operator*() { return pointer_->key; }

    iterator& operator++() {
      if (pointer_->right) {
        pointer_ = min_node();
      } else if (pointer_->parent && pointer_ == pointer_->parent->left) {
        pointer_ = pointer_->parent;
      } else if (pointer_->parent) {
        while (pointer_->parent->parent && pointer_ == pointer_->parent->right)
          pointer_ = pointer_->parent;
        pointer_ = pointer_->parent;
      }
      return *this;
    }

    iterator operator++(int) {
      iterator ret(pointer_);
      ++(*this);
      return ret;
    }

    iterator& operator--() {
      if (pointer_->left) {
        pointer_ = max_node();
      } else if (pointer_->parent && pointer_->parent->left) {
        while (pointer_->parent->parent && pointer_ == pointer_->parent->left)
          pointer_ = pointer_->parent;
        pointer_ = pointer_->parent;
      } else if (pointer_->parent && pointer_->parent->right) {
        pointer_ = pointer_->parent;
      }
      return *this;
    }

    iterator operator--(int) {
      iterator ret(pointer_);
      --(*this);
      return ret;
    }

    bool operator==(const iterator& i) const { return pointer_ == i.pointer_; }
    bool operator!=(const iterator& i) const { return !(*this == i); }

    iterator& operator=(const iterator& i) {
      if (this == &i) return *this;
      pointer_ = i.pointer_;
      return *this;
    }

   private:
    Node* pointer_;

    Node* min_node() {
      Node* min = pointer_->right;
      while (min && min->left) min = min->left;
      return min;
    }

    Node* max_node() {
      Node* max = pointer_->left;
      while (max && max->right) max = max->right;
      return max;
    }
  };

  class const_iterator : public iterator {
    explicit const_iterator(Node* pointer) : iterator(pointer) {}

    const_reference operator*() const {
      return static_cast<iterator*>(this)->operator*();
    }
  };

  BinaryTree() : root(new Node()) {}
  BinaryTree(const BinaryTree& t) : BinaryTree() {
    iterator i = t.begin();
    while (i != t.end()) {
      insert(*i);
      ++i;
    }
  }
  BinaryTree(BinaryTree&& t) : root(nullptr) { std::swap(root, t.root); }
  ~BinaryTree() {
    if (root != nullptr) {
      clear();
      delete root;
      root = nullptr;
    }
  }
  BinaryTree& operator=(BinaryTree&& t) {
    if (&t == this) return *this;
    if (!empty()) {
      clear();
      delete root;
    }
    root = nullptr;
    std::swap(root, t.root);
    return *this;
  }

  iterator begin() const {
    Node* i = root;
    while (i->left) i = i->left;
    return iterator(i);
  }

  iterator end() const {
    Node* i = root;
    while (i->right) i = i->right;
    return iterator(i);
  }

  bool empty() const { return begin() == end(); }

  size_type size() const {
    iterator i = begin();
    size_type res = 0;
    while (i != end()) {
      ++i;
      ++res;
    }
    return res;
  }

  size_type max_size() const {
    return std::numeric_limits<intmax_t>::max() /
           (sizeof(Node) + sizeof(iterator));
  }

  void clear() {
    while (!empty()) {
      erase(begin());
    }
  }

  std::pair<iterator, bool> insert(const value_type& value) {
    if (empty()) {
      insert_root(value);
      return std::pair<iterator, bool>{iterator(this->root), true};
    }
    Node* parent_node = root;
    while ((parent_node->left || parent_node->right) ||
           parent_node->key == value) {
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
    bool not_equal_keys = parent_node->key != value;
    return std::pair<iterator, bool>{
        iterator(
            this->insert_node(value, not_equal_keys ? parent_node : nullptr)),
        not_equal_keys};
  }

  void erase(iterator pos) {
    Node* node = pos.pointer_;
    remove_node(node);
    delete node;
  }

  void swap(BinaryTree& other) { std::swap(root, other.root); }

  iterator find(const T& key) const {
    Node* node = find_node(key);
    return node ? iterator(node) : end();
  }

  bool contains(const T& key) const { return find(key) != end(); }

 protected:
  Node* root;

  void insert_root(const value_type& value) {
    Node* new_root = new Node(value, nullptr);
    new_root->right = root;
    root->parent = new_root;
    root = new_root;
  }

  Node* insert_node(value_type k, Node* p) {
    if (p) {
      Node* node = new Node(k, p);
      if (k < p->key) {
        p->left = node;
      } else {
        if (p->right) {
          node->right = p->right;
          p->right->parent = node;
        }
        p->right = node;
      }
      return node;
    }
    return nullptr;
  }

  void merge_node(iterator i, BinaryTree& other) {
    Node* node = i.pointer_;
    Node* p = find_parent(node->key);
    other.remove_node(node);
    node->parent = p;
    node->left = nullptr;
    node->right = nullptr;
    if (node->key < p->key) {
      p->left = node;
    } else {
      if (p->right) {
        p->right->parent = node;
        node->right = p->right;
      }
      p->right = node;
    }
  }

  void remove_node(Node* node) {
    if (node->left && node->right) {
      remove_two_children_node(node);
    } else if (node->left || node->right) {
      remove_one_child_node(node);
    } else {
      remove_childless_node(node);
    }
  }

 private:
  Node* find_node(const T& key) const {
    Node* node = root;
    while (node && node->key != key) {
      if (key < node->key) {
        node = node->left;
      } else {
        node = node->right;
      }
    }
    return node;
  }

  Node* find_parent(value_type key) const {
    Node* p = root;
    while (p->left || (p->right && iterator(p->right) != end())) {
      if (key < p->key && p->left)
        p = p->left;
      else if (key > p->key && p->right)
        p = p->right;
      else
        break;
    }
    return p;
  }

  void remove_two_children_node(Node* node) {
    Node* replacement = iterator(node->right) == end()
                            ? iterator(node).max_node()
                            : iterator(node).min_node();
    if (replacement == node->right) {
      replacement->left = node->left;
    } else if (replacement == node->left) {
      replacement->right = node->right;
    } else if (replacement->right) {
      replacement->right->parent = replacement->parent;
      replacement->parent->left = replacement->right;
    } else if (replacement->left) {
      replacement->left->parent = replacement->parent;
      replacement->parent->right = replacement->left;
    } else {
      replacement->left = node->left;
      replacement->right = node->right;
      if (replacement == replacement->parent->left)
        replacement->parent->left = nullptr;
      else
        replacement->parent->right = nullptr;
    }
    if (replacement != node->right) {
      node->right->parent = replacement;
    }
    if (replacement != node->left) {
      node->left->parent = replacement;
    }
    if (node == root) {
      root = replacement;
      replacement->parent = nullptr;
    } else {
      if (node == node->parent->left)
        node->parent->left = replacement;
      else
        node->parent->right = replacement;
      replacement->parent = node->parent;
    }
  }

  void remove_one_child_node(Node* node) {
    if (node == root) {
      root = node->left ? node->left : node->right;
    } else {
      if (node == node->parent->left) {
        if (node->left)
          node->parent->left = node->left;
        else
          node->parent->left = node->right;
      } else {
        if (node->left) {
          node->parent->right = node->left;
        } else {
          node->parent->right = node->right;
        }
      }
    }
    if (node->left) {
      node->left->parent = node->left == root ? nullptr : node->parent;
    } else {
      node->right->parent = node->right == root ? nullptr : node->parent;
    }
  }

  void remove_childless_node(Node* node) {
    if (node == node->parent->left)
      node->parent->left = nullptr;
    else
      node->parent->right = nullptr;
  }
};
}  // namespace containers
