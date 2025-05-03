#pragma once
#include <Engine/Core/Interfaces/IComponent.h>
class dataComponent : IComponent {
public:
  ~dataComponent() override;
  dataComponent(void* ptr);
  dataComponent();
private:
  void* _ptr;
};
