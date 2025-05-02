#pragma once
template<typename Mutex>
class UniqueLock
{
public:
  UniqueLock(Mutex& mtx) : m_mtx(mtx), m_isOwned(true) {
    mtx.lock();
  }

  ~UniqueLock() {
    if (m_isOwned) {
      m_mtx.unlock();
    }
  }

  void lock() {
    if (m_isOwned) {
      throw std::runtime_error("mutex already owned");
    }
    m_mtx.lock();
    m_isOwned = true;
  }

  void unlock() {
    if (!m_isOwned) {
      throw std::runtime_error("mutex not locked by this UniqueLock");
    }
    m_mtx.unlock();
    m_isOwned = false;
  }

  inline Mutex* getMutex() {
    return &m_mtx;
  }
private:
  Mutex& m_mtx;
  bool m_isOwned;
};
