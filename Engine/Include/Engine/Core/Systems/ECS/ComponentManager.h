#pragma once
#include <Include/Engine/Core/Interfaces/IComponent.h>
#include <Include/Engine/Core/Systems/ECS/ComponentData.h>
#include <unordered_map>
#include <memory>
#include <typeindex>

class ComponentManager {

public:
  template<typename T>
  void addComponent(Entity entity, const T& component) {
    GetOrCreateComponentData<T>()->addComponent(entity, T(component));
  }

  template<typename T>
  T* GetComponent(Entity entity) {
    auto* data = GetComponentData<T>();
    return data ? data->getComponent(entity) : nullptr;
  }

  template<typename T>
  void removeComponent(Entity entity) {
    auto* data = GetComponentData<T>();
    if (!data || !data->removeComponent(entity)) {
      Logger::Get().logWarning("Entity " + std::to_string(entity) + " does not have this component.");
    }
  }

  template<typename T>
  bool hasComponent(Entity entity) {
    auto* data = GetComponentData<T>();
    return data && data->hasComponent(entity);
  }

  template<typename T>
  ComponentData<T>* GetComponentData() {
    auto it = _componentPools.find(std::type_index(typeid(T)));
    if (it == _componentPools.end()) return nullptr;
    return reinterpret_cast<ComponentData<T>*>(it->second.get());
  }

private:
  struct ComponentDataDeleter {
    void operator()(IComponentData* p) const noexcept {
      if (p) p->destroy();
    }
  };

  template<typename T>
  ComponentData<T>* GetOrCreateComponentData() {
    auto index = std::type_index(typeid(T));
    auto it = _componentPools.find(index);
    if (it == _componentPools.end())
    {
      void* mem = GMalloc->allocate(sizeof(ComponentData<T>), alignof(ComponentData<T>)); // Unknown Tag
      ComponentData<T>* tempPtr = new (mem) ComponentData<T>();
      std::unique_ptr<IComponentData, ComponentDataDeleter> ptr(reinterpret_cast<IComponentData*>(tempPtr));
      _componentPools[index] = std::move(ptr);

      return tempPtr;
    }
    return reinterpret_cast<ComponentData<T>*>(_componentPools[index].get());
  }

private:
  tracked_unordered_map<std::type_index, std::unique_ptr<IComponentData, ComponentDataDeleter>> _componentPools;
};