#pragma once
template<typename CriticalSection>
class UniqueLockCS
{
public:
  UniqueLockCS(CriticalSection& cs) : m_cs(cs), m_isOwned(true) {
    m_cs.lock();
  }

  ~UniqueLockCS() {
    if (m_isOwned) {
      m_cs.unlock();
    }
  }

  void lock() {
    if (m_isOwned) {
      throw std::runtime_error("mutex already owned");
    }
    m_cs.lock();
    m_isOwned = true;
  }

  void unlock() {
    if (!m_isOwned) {
      throw std::runtime_error("mutex not locked by this UniqueLock");
    }
    m_cs.unlock();
    m_isOwned = false;
  }

  inline CriticalSection* getCS() {
    return &m_cs;
  }
private:
  CriticalSection& m_cs;
  bool m_isOwned;
};