#pragma once

template <typename Lockable>
class LockGuard
{
public:
  explicit LockGuard(Lockable& lock) : m_lock(lock) {
    m_lock.lock();
  }

  LockGuard(const LockGuard&) = delete;
  LockGuard& operator=(const LockGuard&) = delete;

  ~LockGuard() {
    m_lock.unlock();
  }

private:
  Lockable& m_lock;
};

