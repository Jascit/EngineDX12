#pragma once
#include <tuple>
#include <Windows.h>
#include <utility>
#include <memory>
#include <stdexcept>

struct Thrd_t {
  void* handle;
  unsigned long id;
};

class thread
{
protected:
  Thrd_t m_thr;
  bool m_isJoined;

public:
  thread() noexcept : m_thr({}), m_isJoined(false) {}
  thread(thread&& other) noexcept // for STD containers
    : m_thr(std::exchange(other.m_thr, {})), m_isJoined(other.m_isJoined) {
  }

  template<typename Fn>
  thread(Fn&& fn) : m_isJoined(false) {
    using FnType = std::decay_t<Fn>;
    FnType* fnPtr = new FnType(std::forward<Fn>(fn));

    m_thr.handle = CreateThread(
      nullptr,
      0,
      ThreadProc<FnType>,
      fnPtr,
      0,
      &m_thr.id
    );

    if (!m_thr.handle)
    {
      delete fnPtr;
      throw std::runtime_error("Failed to create thread");
    }
  };

  thread& operator=(thread&& other) noexcept { // for STD containers
    m_thr = std::exchange(other.m_thr, {});
    return *this;
  }

  template<typename Fn, typename... Args>
  thread(Fn&& fn, Args&&... args) : m_isJoined(false) {
    using FnType = std::decay_t<Fn>;
    using TupleType = std::tuple<FnType, std::decay_t<Args>...>;
    TupleType* tupPtr = new TupleType(std::forward<Fn>(fn), std::forward<Args>(args)...);

    m_thr.handle = CreateThread(
      nullptr,
      0,
      ThreadProcWithParameters<TupleType>,
      tupPtr,
      0,
      &m_thr.id
    );
    if (!m_thr.handle)
    {
      delete tupPtr;
      throw std::runtime_error("Failed to create thread");
    }
  };

  void join() {
    if (!m_isJoined && joinable()) {
      WaitForSingleObject(m_thr.handle, INFINITE);
      m_isJoined = true;
    }
  }

  bool joinable() {
    return m_thr.handle ? true : false;
  }

  ~thread() {
    if (joinable()) {
      CloseHandle(m_thr.handle);
    }
  }
private:
  template<typename Fn>
  static DWORD WINAPI ThreadProc(LPVOID lp) {
    std::unique_ptr<Fn> func(static_cast<Fn*>(lp));
    (*func)();
    return 0;
  }

  template<typename TupleType>
  static DWORD WINAPI ThreadProcWithParameters(LPVOID lp) {
    std::unique_ptr<TupleType> tupPtr(static_cast<TupleType*>(lp));
    std::apply([](auto&& fn, auto&&... args) {
      std::invoke(fn, std::forward<decltype(args)>(args)...);
      }, *tupPtr);

    return 0;
  }
};

