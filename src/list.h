#pragma once

#include <cstddef>
#include <cstdint>
#include <limits>

namespace containers {
template <class T>
class node {
 public:
  node() : value_(T()), next_(nullptr), prev_(nullptr) {}
  explicit node(T value) : value_(T(value)), next_(nullptr), prev_(nullptr) {}
  T& get_value() { return value_; }
  node* get_next() const { return next_; }
  node* get_prev() const { return prev_; }
  void set_next(node* new_next) { next_ = new_next; }
  void set_prev(node* new_prev) { prev_ = new_prev; }

 private:
  T value_;
  node* prev_;
  node* next_;
};

template <class T>
class list {
 public:
  using value_type = T;
  using reference = T&;
  using const_reference = const T&;
  using size_type = size_t;

  class ListIterator {
    friend class list;

   public:
    explicit ListIterator(node<value_type>* pointer = nullptr)
        : pointer_(pointer) {}

    reference operator*() { return pointer_->get_value(); }

    ListIterator& operator++() {
      pointer_ = pointer_->get_next();
      return *this;
    }

    ListIterator operator++(int) {
      ListIterator ret(pointer_);
      ++(*this);
      return ret;
    }

    ListIterator& operator--() {
      pointer_ = pointer_->get_prev();
      return *this;
    }

    ListIterator operator--(int) {
      ListIterator ret(pointer_);
      --(*this);
      return ret;
    }

    ListIterator& operator=(const ListIterator& i) {
      if (this == &i) return *this;
      pointer_ = i.pointer_;
      return *this;
    }

    bool operator==(const ListIterator& i) const {
      return pointer_ == i.pointer_;
    }
    bool operator!=(const ListIterator& i) const { return !(*this == i); }

   protected:
    node<value_type>* pointer_;
  };

  using iterator = ListIterator;

  class ListConstIterator : public iterator {
   public:
    explicit ListConstIterator(node<value_type>* pointer = nullptr)
        : iterator(pointer) {}

    const_reference operator*() const {
      return static_cast<iterator*>(this)->operator*();
    }

    ListConstIterator& operator++() {
      iterator::pointer_ = iterator::pointer_->get_next();
      return *this;
    }

    ListConstIterator operator++(int) {
      ListConstIterator ret(iterator::pointer_);
      ++(*this);
      return ret;
    }

    ListConstIterator& operator--() {
      iterator::pointer_ = iterator::pointer_->get_prev();
      return *this;
    }

    ListConstIterator operator--(int) {
      ListConstIterator ret(iterator::pointer_);
      --(*this);
      return ret;
    }

    ListConstIterator& operator=(const ListConstIterator& i) {
      if (this == &i) return *this;
      iterator::pointer_ = i.pointer_;
      return *this;
    }
  };

  using const_iterator = ListConstIterator;

  list() : front_(new node<value_type>()), back_(front_) {}

  explicit list(size_type n) : list() {
    while (n--) {
      node<value_type>* new_node = new node<value_type>();
      push_back_node(new_node);
    }
  }

  explicit list(std::initializer_list<value_type> const& items) : list() {
    typename std::initializer_list<value_type>::const_iterator i =
        items.begin();
    while (i != items.end()) {
      push_back(*i);
      ++i;
    }
  }

  list(const list& l) : list() {
    iterator i = l.begin();
    while (i != l.end()) {
      push_back(*i);
      ++i;
    }
  }

  list(list&& l) : front_(nullptr), back_(nullptr) {
    std::swap(front_, l.front_);
    std::swap(back_, l.back_);
  }

  ~list() {
    if (front_ != nullptr) {
      clear();
      delete front_;
    }
  }

  list& operator=(list&& l) {
    if (&l == this) return *this;
    if (front_ != nullptr) {
      clear();
      delete front_;
    }
    front_ = nullptr, back_ = nullptr;
    std::swap(front_, l.front_);
    std::swap(back_, l.back_);
    return *this;
  }

  const_reference front() const { return front_->get_value(); }
  const_reference back() const { return back_->get_value(); }

  iterator begin() const { return iterator(front_); }
  const_iterator cbegin() const { return const_iterator(front_); }
  iterator end() const {
    return empty() ? iterator(back_) : iterator(back_->get_next());
  }
  const_iterator cend() const {
    return empty() ? const_iterator(back_) : iterator(back_->get_next());
  }

  bool empty() const {
    return front_ == back_ && front_->get_next() == nullptr;
  }

  size_type size() const {
    size_type result = 0;
    iterator i = begin();
    while (i++ != end()) ++result;
    return result;
  }

  size_type max_size() const {
    return (std::numeric_limits<intmax_t>::max()) / sizeof(node<value_type>);
  }

  void clear() {
    while (!empty()) {
      pop_front();
    }
  }

  iterator insert(iterator pos, const_reference value) {
    if (empty() || pos == begin()) {
      push_front(value);
      return begin();
    } else if (pos == end()) {
      push_back(value);
      return iterator(back_);
    } else {
      node<value_type>* new_node = new node<value_type>(value);
      node<value_type>* cur = pos.pointer_;
      node<value_type>* prev = cur->get_prev();
      new_node->set_prev(prev);
      new_node->set_next(cur);
      prev->set_next(new_node);
      cur->set_prev(new_node);
      return iterator(new_node);
    }
  }

  void erase(iterator pos) {
    node<value_type>* pointer = pos.pointer_;
    if (pointer == front_) {
      pop_front();
    } else if (pointer == back_) {
      pop_back();
    } else {
      node<value_type>* prev = pointer->get_prev();
      node<value_type>* next = pointer->get_next();
      prev->set_next(next);
      next->set_prev(prev);
      delete pointer;
    }
  }

  void push_back(const_reference value) {
    node<value_type>* new_node = new node<value_type>(value);
    push_back_node(new_node);
  }

  void pop_back() {
    if (front_ == back_) {
      front_ = front_->get_next();
      delete back_;
      front_->set_prev(nullptr);
      back_ = front_;
    } else {
      node<value_type>* prev = back_->get_prev();
      node<value_type>* next = back_->get_next();
      prev->set_next(next);
      next->set_prev(prev);
      delete back_;
      back_ = prev;
    }
  }

  void push_front(const_reference value) {
    node<value_type>* new_node = new node<value_type>(value);
    front_->set_prev(new_node);
    new_node->set_next(front_);
    if (empty()) back_ = new_node;
    front_ = new_node;
  }

  void pop_front() {
    node<value_type>* next = front_->get_next();
    next->set_prev(nullptr);
    if (back_ == front_) back_ = back_->get_next();
    delete front_;
    front_ = next;
  }

  void swap(list& other) {
    std::swap(front_, other.front_);
    std::swap(back_, other.back_);
  }

  void merge(list& other) {
    if (empty()) {
      swap(other);
    } else if (this != &other && !(other.empty())) {
      node<value_type>* current = front_;
      node<value_type>* other_next;
      if (other.front() < front()) {
        node<value_type>* other_current = other.front_;
        while (other_current->get_next()->get_value() < front() &&
               iterator(other_current->get_next()) != other.end())
          other_current = other_current->get_next();
        other_next = other_current->get_next();
        other_current->set_next(front_);
        front_->set_prev(other_current);
        front_ = other.front_;
        other.front_ = other_next;
      }
      while (!(other.empty()) && iterator(current) != end()) {
        if (other.front() < current->get_value()) {
          current->get_prev()->set_next(other.front_);
          other.front_->set_prev(current->get_prev());
          other_next = other.front_->get_next();
          other.front_->set_next(current);
          current->set_prev(other.front_);
          other.front_ = other_next;
          if (other.front_->get_next() == nullptr) other.back_ = other.front_;
        } else {
          current = current->get_next();
        }
      }
      if (!(other.empty())) {
        node<value_type>* empty_node = back_->get_next();
        node<value_type>* other_empty = other.back_->get_next();
        back_->set_next(other.front_);
        other.front_->set_prev(back_);
        other.back_->set_next(empty_node);
        empty_node->set_prev(other.back_);
        back_ = other.back_;
        other.front_ = other_empty;
        other.back_ = other.front_;
      }
    }
  }

  void splice(const_iterator pos, list& other) {
    if (!(other.empty())) {
      node<value_type>* pos_pointer = pos.pointer_;
      if (pos != begin())
        pos_pointer->get_prev()->set_next(other.front_);
      else
        front_ = other.front_;
      other.front_->set_prev(pos_pointer->get_prev());
      node<value_type>* other_end = other.back_->get_next();
      other.back_->set_next(pos_pointer);
      pos_pointer->set_prev(other.back_);
      other.front_ = other.back_ = other_end;
    }
  }

  void reverse() {
    if (front_ != back_) {
      node<value_type>* current_node = front_;
      node<value_type>* tmp = back_->get_next();
      tmp->set_prev(front_);
      back_ = front_;
      while (current_node != back_->get_next()) {
        node<value_type>* next_node = current_node->get_next();
        current_node->set_prev(next_node);
        current_node->set_next(tmp);
        tmp = current_node;
        current_node = next_node;
      }
      front_ = tmp;
      back_->set_next(current_node);
    }
  }

  void unique() {
    if (front_ != back_) {
      node<value_type>* prev = front_;
      node<value_type>* cur = front_->get_next();
      while (iterator(cur) != end()) {
        if (cur->get_value() == prev->get_value()) {
          prev->set_next(cur->get_next());
          cur->get_next()->set_prev(prev);
          delete cur;
          cur = prev->get_next();
        } else {
          prev = cur;
          cur = cur->get_next();
        }
      }
    }
  }

  void sort() {
    if (front_ != back_) {
      node<value_type>* prev = front_;
      node<value_type>* cur = front_->get_next();
      while (iterator(cur) != end()) {
        if (cur->get_value() < prev->get_value()) {
          if (cur == back_) back_ = prev;
          node<value_type>* pos = front_;
          while (pos->get_value() <= cur->get_value()) {
            pos = pos->get_next();
          }
          if (pos != front_) pos->get_prev()->set_next(cur);
          cur->set_prev(pos == front_ ? nullptr : pos->get_prev());
          pos->set_prev(cur);
          prev->set_next(cur->get_next());
          cur->get_next()->set_prev(prev);
          cur->set_next(pos);
          if (pos == front_) front_ = cur;
          cur = prev->get_next();
        } else {
          prev = cur;
          cur = cur->get_next();
        }
      }
    }
  }

  template <class... Args>
  iterator emplace(const_iterator pos, Args&&... args) {
    iterator i = pos;
    const value_type data[] = {args...};
    size_t len = sizeof(data) / sizeof(value_type);
    value_type reversed[len];
    for (size_type j = 0; j < len; ++j) reversed[len - j - 1] = data[j];
    for (value_type item : reversed) i = insert(i, item);
    return i;
  }

  template <class... Args>
  void emplace_back(Args&&... args) {
    const value_type data[] = {args...};
    for (const value_type item : data) push_back(item);
  }

  template <class... Args>
  void emplace_front(Args&&... args) {
    const value_type data[] = {args...};
    size_t len = sizeof(data) / sizeof(value_type);
    value_type reversed[len];
    for (size_type j = 0; j < len; ++j) reversed[len - j - 1] = data[j];
    for (value_type item : reversed) push_front(item);
  }

 private:
  node<value_type>*front_, *back_;

  void push_back_node(node<value_type>* node) {
    if (empty()) {
      back_->set_prev(node);
      node->set_next(back_);
      front_ = back_ = node;
    } else {
      node->set_prev(back_);
      node->set_next(back_->get_next());
      back_->get_next()->set_prev(node);
      back_->set_next(node);
      back_ = node;
    }
  }
};
}  // namespace containers
