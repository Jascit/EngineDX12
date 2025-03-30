#pragma once
#include <Windows.h>

template <typename Derived>
class BaseMutex {
protected:
  SRWLOCK m_lock;
public:
  BaseMutex() {
    InitializeSRWLock(&m_lock);
  };
  virtual ~BaseMutex() = default;
  void Lock() {
    static_cast<Derived*>(this)->lockImpl();
  }
  void Unlock() {
    static_cast<Derived*>(this)->unlockImpl();
  }
  inline SRWLOCK* getSRW() {
    return &m_lock;
  }
};
