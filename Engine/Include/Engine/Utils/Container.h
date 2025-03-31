#pragma once
#include <Include/Engine/Utils/WinInclude.h>

class RawContainer {
public:
  RawContainer() : m_size(0), m_data(nullptr) {}

  explicit RawContainer(size_t size) : m_size(size), m_data(nullptr) {
    initialize(m_size);
  }

  RawContainer(const void* data, size_t size) : m_size(size), m_data(nullptr) {
    initialize(m_size);
    if (m_data) {
      memcpy(m_data, data, m_size);
    }
  }

  // Копіюючий конструктор
  RawContainer(const RawContainer& other) : m_size(other.m_size), m_data(nullptr) {
    initialize(m_size);
    if (m_data) {
      memcpy(m_data, other.m_data, m_size);
    }
  }

  // Оператор копіювання
  RawContainer& operator=(const RawContainer& other) {
    if (this == &other) return *this; // Захист від самоприсвоєння

    reset();
    m_size = other.m_size;
    initialize(m_size);
    if (m_data) {
      memcpy(m_data, other.m_data, m_size);
    }
    return *this;
  }

  // Переміщуючий конструктор
  RawContainer(RawContainer&& other) noexcept
    : m_size(other.m_size), m_data(other.m_data) {
    other.m_size = 0;
    other.m_data = nullptr;
  }

  // Оператор переміщення
  RawContainer& operator=(RawContainer&& other) noexcept {
    if (this == &other) return *this; // Захист від самоприсвоєння

    reset();
    m_size = other.m_size;
    m_data = other.m_data;

    other.m_size = 0;
    other.m_data = nullptr;
    return *this;
  }

  ~RawContainer() {
    reset();
  }

  inline size_t GetSize() const { return m_size; }
  inline void* GetData() const { return m_data; }

  void reset() {
    if (m_data) {
      free(m_data);
      m_data = nullptr;
      m_size = 0;
    }
  }

  bool initialize(size_t size) {
    if (!m_data) {
      m_size = size;
      m_data = malloc(m_size);
      if (!m_data) {
        Logger::Get().logError("Failed to malloc data");
        return false;
      }
      return true;
    }
    Logger::Get().logError("Data is still in use, cannot be initialized");
    return false;
  }

private:
  size_t m_size;
  void* m_data;
};
