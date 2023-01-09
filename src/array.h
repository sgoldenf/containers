#pragma once

#include <stdexcept>

namespace containers {
template <class T, long unsigned int capacity>
class array {
 public:
  using value_type = T;
  using reference = T &;
  using const_reference = const T &;
  using size_type = size_t;
  using iterator = T *;
  using const_iterator = const T *;

 private:
  value_type *begin_;
  value_type data_[capacity];

 public:
  array() {
    data_[capacity] = {};
    begin_ = &data_[0];
  }

  explicit array(std::initializer_list<value_type> const &items) {
    begin_ = &data_[0];
    typename std::initializer_list<value_type>::iterator j = items.begin();
    for (size_type i = 0; i < items.size() && j != items.end(); ++i, ++j)
      data_[i] = *j;
  }

  array(const array &v) {
    begin_ = &data_[0];
    for (size_t i = 0; i < capacity; ++i) data_[i] = v.data_[i];
  }

  array(array &&v) : begin_(nullptr) {
    for (size_t i = 0; i < capacity; ++i) {
      data_[i] = std::move(v.data_[i]);
    }
    std::swap(begin_, v.begin_);
  }

  ~array() { begin_ = nullptr; }

  array &operator=(array &&v) {
    if (&v == this) return *this;
    std::swap(begin_, v.begin_);
    for (size_t i = 0; i < capacity; ++i) {
      data_[i] = std::move(v.data_[i]);
    }
    return *this;
  }

  reference at(size_type pos) {
    if (pos >= capacity)
      throw std::out_of_range(
          "containers::array::_M_range_check: __n >= this->size()");
    return data_[pos];
  }

  reference operator[](size_type pos) { return data_[pos]; }

  const_reference front() { return *begin_; }
  const_reference back() { return *(begin_ + capacity - 1); }

  iterator data() { return begin(); }

  iterator begin() const { return iterator(begin_); }
  iterator end() const { return iterator(begin_ + capacity); }

  bool empty() { return capacity == 0 ? true : false; }

  size_type size() const { return capacity; }

  size_type max_size() { return capacity; }

  void swap(array &other) { std::swap(data_, other.data_); }

  void fill(const_reference value) {
    for (size_t i = 0; i < capacity; ++i) {
      data_[i] = value;
    }
  }
};

}  // namespace containers
