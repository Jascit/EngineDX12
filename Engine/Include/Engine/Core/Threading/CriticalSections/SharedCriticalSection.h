#pragma once
#include <Windows.h>
#include <atomic>

class SharedCriticalSection {
public:
  SharedCriticalSection() : _exclusiveLocked(false) {
    InitializeCriticalSection(&_cs);
    InitializeConditionVariable(&_cv);
  }

  ~SharedCriticalSection() {
    DeleteCriticalSection(&_cs);
  }

  // Exklusiver Lock (writer)
  void lock() {
    _writerCount++;
    EnterCriticalSection(&_cs);
    _exclusiveLocked = true;
  }

  void unlock() {
    _writerCount--;
    _exclusiveLocked = false;
    WakeAllConditionVariable(&_cv);
    LeaveCriticalSection(&_cs);
  }

  // Shared Lock (reader)
  void lockShared() {
    _readerCount++;
    EnterCriticalSection(&_cs);
    while (_exclusiveLocked && _writerCount.load() > 0) {
      // Warte solange, bis kein writer aktiv ist
      SleepConditionVariableCS(&_cv, &_cs, INFINITE);
    }
    _readerCount--;
  }

  void unlockShared() {
    LeaveCriticalSection(&_cs);
  }

  inline CRITICAL_SECTION* getCS() {
    return &_cs;
  }
private:
  CRITICAL_SECTION _cs;
  CONDITION_VARIABLE _cv;
  bool _exclusiveLocked;
  std::atomic<uint32_t> _writerCount{0};
  std::atomic<uint32_t> _readerCount{0};
};
