#pragma once
class IComponentData {
public:
  virtual ~IComponentData() = default;
  virtual void destroy() noexcept = 0;
};