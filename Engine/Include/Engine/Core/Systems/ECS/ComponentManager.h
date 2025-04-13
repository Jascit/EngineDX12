#pragma once
#include <Include/Engine/Core/Interfaces/IComponent.h>
#include "ComponentData.h"
#include <unordered_map>
#include <memory>
#include <typeindex>

class ComponentManager {
public:
  template<typename T>
  void addComponent(Entity entity, const T& component) {
    static_assert(std::is_base_of<IComponent, T>::value, "Component must be derived from IComponent");

    getComponentData<T>().addComponent(entity, std::decay_t<T>(component));
  }

  template<typename T>
  T* getComponent(Entity entity) {
    auto& componentMap = getComponentData<T>();
    auto* ptr = componentMap.getComponent(entity);
    if (ptr == nullptr) {
      return nullptr;
    }
    return ptr;
  }

  template<typename T>
  void removeComponent(Entity entity) {
    auto& componentMap = getComponentData<T>();
    if (componentMap.removeComponent(entity) == false) {
      Logger::Get().logWarning("Warning: Entity " + std::to_string(entity) + " does not have this component.");
    }
  }

  template<typename T>
  bool hasComponent(Entity entity) {
    return getComponentData<T>().hasComponent(entity);
  }

  template<typename T>
  void clearComponents() {
    getComponentData<T>().clear();
  }

  template<typename T>
  ComponentData<T>& getComponentData() {
    static ComponentData<T> typedComponentMap;
    return typedComponentMap;
  }

};
