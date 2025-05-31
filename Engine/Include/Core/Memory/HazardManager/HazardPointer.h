//#pragma once
//
//#include <atomic>
//#include <thread>
//#include <stdexcept>
//
//class HazardPointer {
//public:
//  HazardPointer() : m_hp(nullptr) {}
//  HazardPointer(void* ptr) : m_hp(ptr) {}
//  HazardPointer(const HazardPointer&) = delete;
//  HazardPointer& operator=(const HazardPointer&) = delete;
//
//
//  void protect(void* ptr) {
//    m_hp.store(ptr, std::memory_order_release);
//  }
//
//  void release() {
//    m_hp.store(nullptr, std::memory_order_release);
//  }
//
//  void* Get() const {
//    return m_hp.load(std::memory_order_acquire);
//  }
//
//  friend bool operator==(const HazardPointer& lhs, const HazardPointer& rhs) {
//    return lhs.Get() == rhs.Get();
//  }
//
//  friend bool operator!=(const HazardPointer& lhs, const HazardPointer& rhs) {
//    return !(lhs.Get() == rhs.Get());
//  }
//
//  friend bool operator==(const HazardPointer& lhs, const void* ptr) {
//    return lhs.Get() == ptr;
//  }
//
//  friend bool operator==(const void* ptr, const HazardPointer& rhs) {
//    return rhs == ptr;
//  }
//
//  friend bool operator!=(const HazardPointer& lhs, const void* ptr) {
//    return !(lhs.Get() == ptr);
//  }
//
//  friend bool operator!=(const void* ptr, const HazardPointer& rhs) {
//    return !(ptr == rhs.Get());
//  }
//
//private:
//  std::atomic<void*> m_hp;
//};
