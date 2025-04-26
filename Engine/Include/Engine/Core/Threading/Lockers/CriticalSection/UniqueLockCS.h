#pragma once
#include <stdexcept>  // For std::runtime_error

template<typename CriticalSection>
class UniqueLockCS
{
public:
  // Delete copy constructor and copy assignment operator.
  UniqueLockCS(const UniqueLockCS&) = delete;
  UniqueLockCS& operator=(const UniqueLockCS&) = delete;

  // Constructor: locks the CriticalSection.
  UniqueLockCS(CriticalSection& cs)
    : m_cs(cs), m_isOwned(true)
  {
    m_cs.lock();
  }

  // Destructor: unlocks the CriticalSection if owned.
  ~UniqueLockCS() {
    if (m_isOwned) {
      m_cs.unlock();
    }
  }

  // Lock the CriticalSection if not already locked.
  void lock() {
    if (m_isOwned) {
      throw std::runtime_error("mutex already owned");
    }
    m_cs.lock();
    m_isOwned = true;
  }

  // Unlock the CriticalSection if currently locked.
  void unlock() {
    if (!m_isOwned) {
      throw std::runtime_error("mutex not locked by this UniqueLock");
    }
    m_cs.unlock();
    m_isOwned = false;
  }

  // Get a pointer to the underlying CriticalSection.
  inline CriticalSection* getCS() {
    return &m_cs;
  }

private:
  CriticalSection& m_cs;
  bool m_isOwned;
};