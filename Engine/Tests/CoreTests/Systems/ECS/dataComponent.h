#pragma once
#include <Include/Engine/Core/Interfaces/IComponent.h>
class dataComponent : IComponent {
public:
  ~dataComponent() override {};
  dataComponent(void* ptr) : _ptr(ptr) {};
  dataComponent() : _ptr(nullptr) {};
private:
  void* _ptr;
};