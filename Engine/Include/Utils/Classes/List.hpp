#include <Core/Memory/STLAllocator.hpp>
#include <atomic>

//free indices, objects
enum class ListType {
  NONE = 0, // no optimisation
  PUSH_FRONT_ONLY_POP_BACK,
  PUSH_BACK_ONLY_POP_FRONT
};

// base template class List
template <typename T, ListType type = ListType::NONE>
class List;

// spezialisation for ListType::NONE
template <typename T>
class List<T, ListType::NONE> {
  enum ListFlag : uint32_t {
    NONE = 0,
    TO_DESTROY
  };
  struct Node {
    T _data;
    std::atomic<uint32_t> _readerCounter;
    std::atomic<Node*> _last;
    std::atomic<Node*> _next;

    Node(T&& data) : _data(std::forward<T>(data)), _last(nullptr), _next(nullptr), _readerCount{0} {};
  };

public:

  List(ListType type = ListType::NONE) {
    
  }

  void push_front(T&& data) {
    ListType type = ListType::NONE;
    if (type == ListType::NONE)
    {
      this->default_pop_return() = delete;
    }
  }

  void push_back(T&& data) {

  }
  
  const T pop_back_return() {
   
  }

  const T pop_front_return() {
   
  }

  bool empty() noexcept {
    return m_size.load(std::memory_order_acquire) ? false : true;
  }

  inline uint32_t size() noexcept {
    return m_size.load(std::memory_order_acquire);
  }

private:
  T default_pop_return() {
    uint32_t size = m_size.fetch_sub(1, std::memory_order_acq_rel);
    if (size < 0) {
      m_size.fetch_add(1, std::memory_order_release);
    }
    else if (size == 0) {

    }

    if (m_headSize.load(std::memory_order_acquire) >= m_tailSize.load(std::memory_order_acquire))
    {

    }
  };
private:
  std::atomic<uint32_t> m_size;
  std::atomic<uint32_t> m_tailSize;
  std::atomic<uint32_t> m_headSize;
  std::atomic<Node*> m_synchPoint;
  std::atomic<Node*> m_head;
  std::atomic<Node*> m_tail;

};

// spezialisation for ListType::POP_ON_BACK_ONLY_PUSH_FRONT
template <typename T>
class List<T, ListType::PUSH_FRONT_ONLY_POP_BACK> {
  enum ListFlag : uint32_t {
    NONE = 0,
    TO_DESTROY
  };
  struct Node {
    T _data;
    std::atomic<uint32_t> _readerCounter;
    std::atomic<Node*> _last;
    std::atomic<Node*> _next;

    Node(T&& data) : _data(std::forward<T>(data)), _last(nullptr), _next(nullptr), _readerCount{ 0 } {};
  };
public:

  void push_front(T&& data) {
    Node* instance = GMalloc->allocate(sizeof(T), alignof(T));
    GMalloc->construct(instance, data);
    if (m_size.load(std::memory_order_acquire) == 0)
    {
      Node* expected = nullptr;
      if (m_head.compare_exchange_strong(expected, instance));
      {
        m_tail.store()
      }
    }
  }

  const T pop_back_return() {

  }

  bool empty() noexcept {
    return m_size.load(std::memory_order_acquire) ? false : true;
  }

  inline uint32_t size() noexcept {
    return m_size.load(std::memory_order_acquire);
  }
private:
  std::atomic<uint32_t> m_size;
  std::atomic<Node*> m_head;
  std::atomic<Node*> m_tail;
};

// spezialisation for ListType::POP_ON_FRONT_ONLY_PUSH_BACK
template <typename T>
class List<T, ListType::PUSH_BACK_ONLY_POP_FRONT> {
  enum ListFlag : uint32_t {
    NONE = 0,
    TO_DESTROY
  };
  struct Node {
    T _data;
    std::atomic<uint32_t> _readerCounter;
    std::atomic<Node*> _last;
    std::atomic<Node*> _next;

    Node(T&& data) : _data(std::forward<T>(data)), _last(nullptr), _next(nullptr), _readerCount{ 0 } {};
  };
public:
  void push_back(T&& data) {

  }
  
  const T pop_front_return() {

  }  
  
  bool empty() noexcept {
    return m_size.load(std::memory_order_acquire) ? false : true;
  }

  inline uint32_t size() noexcept {
    return m_size.load(std::memory_order_acquire);
  }
private:

};