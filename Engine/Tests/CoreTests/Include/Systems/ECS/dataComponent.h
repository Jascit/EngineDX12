#pragma once
#include <Core/Interfaces/IComponent.h>
class dataComponent : IComponent {
public:
  ~dataComponent() override;
  dataComponent(void* ptr);
  dataComponent();
private:
  void* _ptr;
};
