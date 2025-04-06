#include <atomic>

class BaseGarbageObject {
public:
  BaseGarbageObject() : m_ref(1), m_deadBytes(0) {}
  virtual ~BaseGarbageObject() = default;
  virtual void shutdown() = 0;

  size_t addRef() {
    return m_ref.fetch_add(1, std::memory_order_relaxed) + 1;
  }

  virtual void reset() {
    m_ref.store(1, std::memory_order_relaxed);
  }

  void release() {
    if (m_ref.fetch_sub(1, std::memory_order_release) == 1) {
      //GCManager::Get().killMemory(this);
    }
  }

  void SetDeadBytes(size_t newValue) {
    if (newValue > 0) {
      m_deadBytes = newValue;
    }
    else {
      
    }
  }

protected:
  size_t m_deadBytes;
  std::atomic<size_t> m_ref;
};