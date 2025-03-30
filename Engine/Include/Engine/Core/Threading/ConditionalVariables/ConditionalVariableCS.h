#pragma once
#include <Windows.h>
#include <Include/Engine/Core/Threading/CriticalSections/CriticalSection.h>
#include <Include/Engine/Core/Threading/Lockers/CriticalSection/UniqueLockCS.h>
class ConditionalVariableCS
{
public:
  ConditionalVariableCS() {
    InitializeConditionVariable(&m_cv);
  }
  template<typename Pred>
  void wait(UniqueLockCS<CriticalSection>& cs, Pred pred) {
    while (!pred())
    {
      SleepConditionVariableCS(&m_cv, cs.getCS()->getCS(), INFINITE);
    }
  }
  void notifyOne() {
    WakeConditionVariable(&m_cv);
  }
  void notifyAll() {
    WakeAllConditionVariable(&m_cv);
  }
private:
  CONDITION_VARIABLE m_cv;

};