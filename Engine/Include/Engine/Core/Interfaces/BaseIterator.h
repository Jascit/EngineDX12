#pragma once
template<typename T, typename Derived>
class BaseIterator {
protected:
  T* m_ptr;
public:
  explicit BaseIterator(T* ptr) : m_ptr(m_ptr) {}
  virtual void operator++() {
    static_cast<Derived*>(this)->operator++();
  }
  virtual void operator--() {
    static_cast<Derived*>(this)->operator--();
  }
  bool operator==(const BaseIterator& other) {
    return m_ptr == other.m_ptr;
  }
  bool operator!=(const BaseIterator& other) {
    return m_ptr != other.m_ptr;
  }
  T& operator*() { return *m_ptr; }

  T* operator->() {
    return m_ptr;
  }

  T* get() const {
    return m_ptr;
  }

  void set(T* ptr) {
    m_ptr = ptr;
  }
};