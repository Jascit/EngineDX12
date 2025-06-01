#pragma once
#include <Core/Interfaces/IComponent.h>
class sumComponent : IComponent {
public:
  ~sumComponent() override {};
  sumComponent(size_t sum) : _sum(sum) {};
  sumComponent() : _sum(0) {};
private:
  size_t _sum;
};
