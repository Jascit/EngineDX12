#pragma once
#include <Include/Engine/Core/Interfaces/IComponent.h>
class PositionComponent : IComponent{
public:
  ~PositionComponent() override {};
  PositionComponent(size_t x, size_t y) : m_x(x), m_y(y) {};
  PositionComponent() : m_x(0), m_y(0) {};
private:
  size_t m_x, m_y;
};