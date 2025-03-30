#pragma once
#include <string>
#include <atomic>
class BaseUnknown {
protected:
  std::string m_name;
public:
  virtual inline const std::string& GetName() const { return m_name; };
  virtual void SetName(const std::string& name) { m_name = name; };
  BaseUnknown() : m_name("Unknown") {}
  BaseUnknown(const std::string& name) : m_name(name) {}
  BaseUnknown(const std::string& name, std::atomic<uint32_t>& ID) : m_name(name + "_" + std::to_string(ID.fetch_add(1, std::memory_order_relaxed))) {}
  virtual void shutdown() = 0;
};