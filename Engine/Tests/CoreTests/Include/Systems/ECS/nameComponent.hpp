#pragma once
#include <Core/Interfaces/IComponent.h>
class nameComponent : IComponent {
public:
  ~nameComponent() override {};
  nameComponent(std::string name) : _name(name) {};
  nameComponent() : _name("Unknown") {};
private:
  std::string _name;
};
