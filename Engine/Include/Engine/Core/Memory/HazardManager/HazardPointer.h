#pragma once

#include <atomic>
#include <thread>
#include <stdexcept>

class HazardPointer {
public:
  HazardPointer() : m_ptr(nullptr), m_owner(std::thread::id()) {}
  HazardPointer(void* ptr) : m_ptr(ptr), m_owner(std::this_thread::get_id()) {}
  HazardPointer(const HazardPointer&) = delete;
  HazardPointer& operator=(const HazardPointer&) = delete;


  void protect(void* ptr) {
    hp->pointer.store(ptr, std::memory_order_release);
  }

  void release() {
    hp->pointer.store(nullptr, std::memory_order_release);
  }

  void* Get() const {
    return m_ptr.load(std::memory_order_acquire);
  }

  std::thread::id GetOwner() const {
    return m_owner.load(std::memory_order_acquire);
  }

  void SetOwner(const std::thread::id& tid) {
    m_owner.store(tid, std::memory_order_release);
  }

  friend bool operator==(const HazardPointer& lhs, const HazardPointer& rhs) {
    return lhs.Get() == rhs.Get();
  }

  friend bool operator!=(const HazardPointer& lhs, const HazardPointer& rhs) {
    return !(lhs.Get() == rhs.Get());
  }

  friend bool operator==(const HazardPointer& lhs, const void* ptr) {
    return lhs.Get() == ptr;
  }

  friend bool operator==(const void* ptr, const HazardPointer& rhs) {
    return rhs == ptr;
  }

  friend bool operator!=(const HazardPointer& lhs, const void* ptr) {
    return !(lhs.Get() == ptr);
  }

  friend bool operator!=(const void* ptr, const HazardPointer& rhs) {
    return !(ptr == rhs.Get());
  }

private:
  std::atomic<void*> m_ptr;
  std::atomic<std::thread::id> m_owner;
};