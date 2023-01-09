#pragma once

#include <cstdint>
#include <limits>
#include <stdexcept>

namespace containers {
template <class T>
class vector {
 public:
  using value_type = T;
  using reference = T&;
  using const_reference = const T&;
  using size_type = size_t;

  class VectorIterator {
   public:
    explicit VectorIterator(value_type* pointer = nullptr)
        : pointer_(pointer) {}

    reference operator*() { return *pointer_; }

    VectorIterator& operator++() {
      ++pointer_;
      return *this;
    }

    VectorIterator operator++(int) {
      VectorIterator ret(pointer_);
      ++(*this);
      return ret;
    }

    VectorIterator& operator--() {
      --pointer_;
      return *this;
    }

    VectorIterator operator--(int) {
      VectorIterator ret(pointer_);
      --(*this);
      return ret;
    }

    bool operator==(const VectorIterator& i) const {
      return pointer_ == i.pointer_;
    }
    bool operator!=(const VectorIterator& i) const { return !(*this == i); }

    VectorIterator& operator=(const VectorIterator& i) {
      if (this == &i) return *this;
      pointer_ = i.pointer_;
      return *this;
    }

   protected:
    value_type* pointer_;
  };

  using iterator = VectorIterator;

  class VectorConstIterator : public iterator {
   public:
    explicit VectorConstIterator(value_type* pointer = nullptr)
        : iterator(pointer) {}

    const_reference operator*() const { return *(iterator::pointer_); }

    VectorConstIterator& operator++() {
      ++(iterator::pointer_);
      return *this;
    }

    VectorConstIterator operator++(int) {
      VectorConstIterator ret(iterator::pointer_);
      ++(*this);
      return ret;
    }

    VectorConstIterator& operator--() {
      --(iterator::pointer_);
      return *this;
    }

    VectorConstIterator operator--(int) {
      VectorConstIterator ret(iterator::pointer_);
      --(*this);
      return ret;
    }

    VectorConstIterator& operator=(const VectorConstIterator& i) {
      if (this == &i) return *this;
      iterator::pointer_ = i.pointer_;
      return *this;
    }
  };

  using const_iterator = VectorConstIterator;

  vector() : data_(nullptr), back_(data_), capacity_(0) {}

  explicit vector(size_type n)
      : data_(new value_type[n]()), back_(data_ + n - 1), capacity_(n) {}

  explicit vector(std::initializer_list<value_type> const& items)
      : vector(items.size()) {
    typename std::initializer_list<value_type>::iterator j = items.begin();
    for (size_type i = 0; i < items.size() && j != items.end(); ++i, ++j)
      data_[i] = *j;
  }

  vector(const vector& v) : vector(v.size()) {
    iterator i = begin();
    iterator j = v.begin();
    while (i != end()) *(i++) = *(j++);
  }

  vector(vector&& v) : data_(nullptr), back_(nullptr), capacity_(0) {
    std::swap(data_, v.data_);
    std::swap(back_, v.back_);
    std::swap(capacity_, v.capacity_);
  }

  ~vector() {
    if (data_ != nullptr) delete[] data_;
    data_ = nullptr;
    back_ = nullptr;
    capacity_ = 0;
  }

  vector& operator=(vector&& v) {
    if (&v == this) return *this;
    if (data_ != nullptr) delete[] data_;
    data_ = nullptr, back_ = nullptr;
    capacity_ = 0;
    std::swap(data_, v.front_);
    std::swap(back_, v.back_);
    std::swap(capacity_, v.capacity_);
    return *this;
  }

  reference at(size_type pos) {
    if (pos >= size())
      throw std::out_of_range(
          "containers::vector::_M_range_check: __n >= this->size()");
    return data_[pos];
  }

  reference operator[](size_type pos) { return data_[pos]; }

  const_reference front() { return *data_; }
  const_reference back() { return *back_; }

  value_type* data() { return data_; }

  iterator begin() const { return iterator(data_); }
  iterator end() const {
    return back_ == nullptr ? iterator(data_) : iterator(back_ + 1);
  }
  const_iterator cbegin() const { return const_iterator(data_); }
  const_iterator cend() const {
    return back_ == nullptr ? const_iterator(data_) : const_iterator(back_ + 1);
  }

  bool empty() { return begin() == end(); }

  size_type size() const {
    size_type result = 0;
    iterator i = begin();
    while (i != end()) {
      ++result;
      ++i;
    }
    return result;
  }

  size_type max_size() {
    return (std::numeric_limits<intmax_t>::max()) / sizeof(value_type);
  }

  void reserve(size_type size) {
    if (size > capacity_) reallocate(size);
  }

  size_type capacity() { return capacity_; }

  void shrink_to_fit() {
    if (capacity_ != size()) reallocate(size());
  }

  void clear() { back_ = nullptr; }

  iterator insert(iterator pos, const_reference value) {
    value_type* ret;
    if (pos != end()) {
      value_type* i;
      if (size() == capacity_) {
        capacity_ = capacity_ ? capacity_ * 2 : 1;
        value_type* new_data = new value_type[capacity_];
        i = new_data;
        for (value_type* j = data_; iterator(j) != end(); ++i) {
          if (iterator(j) == pos) {
            ret = i;
            *(i++) = value;
          }
          *i = *(j++);
        }

        back_ = --i;
        delete[] data_;
        data_ = new_data;
      } else {
        i = back_ + 1;
        while (iterator(i) != pos) {
          value_type previous = *(i - 1);
          *(i--) = previous;
        }
        *i = value;
        back_ = back_ + 1;
        ret = i;
      }
    } else {
      push_back(value);
      ret = back_;
    }
    return iterator(ret);
  }

  void erase(iterator pos) {
    if (pos == --(end())) {
      pop_back();
    } else {
      iterator i = pos;
      while (i != end()) {
        iterator tmp = i++;
        *tmp = *i;
      }
      back_ = back_ - 1;
    }
  }

  void push_back(const_reference value) {
    size_type old_size = this->size();
    if (size() == capacity_) reserve(capacity_ ? capacity_ * 2 : 1);
    if (old_size) {
      data_[old_size] = value;
      back_ = data_ + old_size;
    } else {
      *data_ = value;
      back_ = data_;
    }
  }

  void pop_back() {
    if (size() == 1)
      back_ = nullptr;
    else
      back_ = back_ - 1;
  }

  void swap(vector& other) {
    std::swap(data_, other.data_);
    std::swap(back_, other.back_);
    std::swap(capacity_, other.capacity_);
  }

  template <class... Args>
  iterator emplace(const_iterator pos, Args&&... args) {
    iterator i = pos;
    const value_type items[] = {args...};
    size_t len = sizeof(items) / sizeof(value_type);
    value_type reversed[len];
    for (size_type j = 0; j < len; ++j) reversed[len - j - 1] = items[j];
    for (const value_type item : reversed) i = insert(i, item);
    return i;
  }

  template <class... Args>
  void emplace_back(Args&&... args) {
    const value_type items[] = {args...};
    for (const value_type item : items) push_back(item);
  }

 private:
  value_type *data_, *back_;
  size_type capacity_;

  void reallocate(size_type size) {
    value_type* new_data = new value_type[size];
    if (!empty()) {
      for (size_type i = 0; i < this->size(); ++i) new_data[i] = data_[i];
    }
    back_ = this->size() ? new_data + this->size() - 1 : new_data;
    if (data_ != nullptr) delete[] data_;
    data_ = new_data;
    capacity_ = size;
  }
};
}  // namespace containers
