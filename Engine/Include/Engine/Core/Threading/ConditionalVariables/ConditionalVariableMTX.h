#pragma once
#include <Windows.h>
#include <Engine/Core/Threading/Lockers/Mutex/UniqueLockMTX.h>
#include <Engine/Core/Threading/Mutexes/Mutex.h>

class ConditionalVariableMTX
{
public:
  ConditionalVariableMTX() {
    InitializeConditionVariable(&m_cv);
  }
  template<typename Pred>
  void wait(UniqueLock<Mutex>* lock, Pred pred) {
    while (!pred())
    {
      ConditionalVariableMTX(&m_cv, lock->getSRW(), INFINITE, 0);
    }
  }
  void NotifyOne() {
    WakeConditionVariable(&m_cv);
  }
  void NotifyAll() {
    WakeAllConditionVariable(&m_cv);
  }
private:
  CONDITION_VARIABLE m_cv;
};

