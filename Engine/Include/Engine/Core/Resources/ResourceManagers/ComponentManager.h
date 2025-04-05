#pragma once
#include <Include/Engine/Core/Resources/ResourceManagers/EntityManager.h>
#include <Include/Engine/Core/Resources/Components/IComponents.h>
#include <unordered_map>
#include <memory>
#include <typeindex>

class ComponentManager {
public:
  template<typename T>
  void addComponent(Entity entity, const T& component) {
    static_assert(std::is_base_of<IComponent, T>::value, "Component must be derived from IComponent");

    auto& componentMap = getComponentMap<T>();
    componentMap[entity] = component;
  }

  template<typename T>
  T* getComponent(Entity entity) {
    auto& componentMap = getComponentMap<T>();
    auto it = componentMap.find(entity);
    if (it == componentMap.end()) {
      Logger::Get().logWarning("Component not found for entity " + std::to_string(entity));
      return nullptr;
    }
    return it->second.get();
  }

  template<typename T>
  void removeComponent(Entity entity) {
    auto& componentMap = getComponentMap<T>();
    if (componentMap.erase(entity) == 0) {
      Logger::Get().logWarning("Warning: Entity " + std::to_string(entity) + " does not have this component.");
    }
  }

  template<typename T>
  bool hasComponent(Entity entity) {
    auto& componentMap = getComponentMap<T>();
    return componentMap.find(entity) != componentMap.end();
  }

  template<typename T>
  void clearComponents() {
    getComponentMap<T>().clear();
  }

  template<typename T>
  std::unordered_map<Entity, T>& getComponentMap() {
    static std::unordered_map<Entity, T> typedComponentMap;
    return typedComponentMap;
  }

};
