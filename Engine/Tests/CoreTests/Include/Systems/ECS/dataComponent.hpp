#pragma once
#include <Core/Interfaces/IComponent.hpp>
class dataComponent : IComponent {
public:
  ~dataComponent() override;
  dataComponent(void* ptr);
  dataComponent();
private:
  void* _ptr;
};
