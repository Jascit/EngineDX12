#pragma once
#include <Windows.h>

class CriticalSection {
public:
  CriticalSection() {
    InitializeCriticalSection(&m_cs);
  }

  ~CriticalSection() {
    DeleteCriticalSection(&m_cs);
  }

  void lock() {
    EnterCriticalSection(&m_cs);
  }


  void unlock() {
    LeaveCriticalSection(&m_cs);
  }
  inline CRITICAL_SECTION* getCS() {
    return &m_cs;
  }
private:
  CRITICAL_SECTION m_cs;
};
