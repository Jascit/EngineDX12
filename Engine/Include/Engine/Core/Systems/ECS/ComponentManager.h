#pragma once
#include <Engine/Core/Threading/CriticalSections/CriticalSection.h>
#include <Engine/Core/Threading/Lockers/LockGuard.h>
#include <Engine/Core/Systems/ECS/ComponentData.h>
#include <Engine/Core/Interfaces/IComponent.h>
#include <unordered_map>
#include <memory>
#include <typeindex>
// TODO: 2, lock free struckt 
class ComponentManager {

public:
  template<typename T>
  void addComponent(Entity entity, const T& component) {
    GetOrCreateComponentData<T>()->addComponent(entity, T(component));
  }

  template<typename T>
  T* GetComponent(Entity entity) {
    ComponentData<T>* data = GetComponentData<T>();
    return data ? data->GetComponent(entity) : nullptr;
  }

  template<typename T>
  void removeComponent(Entity entity) {
    ComponentData<T>* data = GetComponentData<T>();
    if (!data || !data->removeComponent(entity)) {
      Logger::Get().logWarning("Entity " + std::to_string(entity) + " does not have this component.");
    }
  }

  template<typename T>
  bool hasComponent(Entity entity) {
    ComponentData<T>* data = GetComponentData<T>();
    return data && data->hasComponent(entity);
  }

  template<typename T>
  ComponentData<T>* GetComponentData() {
    static ComponentData<T>* hashed = nullptr;
    if (hashed) return hashed;
    LockGuard<CriticalSection> lock(_csB);
    if (hashed) return hashed;
    auto index = std::type_index(typeid(T));
    auto it = _componentPools.find(std::type_index(typeid(T)));
    if (it == _componentPools.end()) return nullptr;
    hashed = reinterpret_cast<ComponentData<T>*>(it->second.get());
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
    static ComponentData<T>* hashed = nullptr;
    if (hashed) return hashed;
    LockGuard<CriticalSection> lock(_csA);
    if (hashed) return hashed;
    auto index = std::type_index(typeid(T));
    auto it = _componentPools.find(index);
    if (it == _componentPools.end())
    {
      void* mem = GMalloc->allocate(sizeof(ComponentData<T>), alignof(ComponentData<T>)); // Unknown Tag
      ComponentData<T>* tempPtr = new (mem) ComponentData<T>();
      std::unique_ptr<IComponentData, ComponentDataDeleter> ptr(reinterpret_cast<IComponentData*>(tempPtr));
      _componentPools[index] = std::move(ptr);

      hashed = reinterpret_cast<ComponentData<T>*>(_componentPools[index].get());
      return tempPtr;
    }
    return reinterpret_cast<ComponentData<T>*>(_componentPools[index].get());
  }

private:
  tracked_unordered_map<std::type_index, std::unique_ptr<IComponentData, ComponentDataDeleter>> _componentPools;
  CriticalSection _csA;
  CriticalSection _csB;
};
