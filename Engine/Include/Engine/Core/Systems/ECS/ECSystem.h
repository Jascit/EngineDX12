#pragma once
#include <Include/Engine/Core/Interfaces/ISystem.h>
#include "EntityManager.h"
#include "ComponentManager.h"
// The ECSystem encapsulates the work with entities and components.
// It serves as a central interface for the Entity Component System.
class ECSystem : public ISystem {
public:
  void initialize() override;
  void update() override;
  void shutdown() override;

  // --- Entity-Logic ---
  Entity createEntity();
  void removeEntity(Entity entity);
  const std::vector<Entity>& getActiveEntities() const;

  // --- Components-Logic ---
  template<typename T>
  void addComponent(Entity entity, const T& component);

  template<typename T>
  T* getComponent(Entity entity);

  template<typename T>
  bool hasComponent(Entity entity);

  // TODO: 2 Communication with GCS to delete unneeded components and entities
  template<typename T>
  void removeComponent(Entity entity);

  template<typename T>
  ComponentData<T>& getComponentData();

private:
  EntityManager m_entityManager;
  ComponentManager m_componentManager;
};

template<typename T>
void ECSystem::addComponent(Entity entity, const T& component) {
  m_componentManager.addComponent<T>(entity, component);
}

template<typename T>
T* ECSystem::getComponent(Entity entity) {
  return m_componentManager.getComponent<T>(entity);
}

template<typename T>
bool ECSystem::hasComponent(Entity entity) {
  return m_componentManager.hasComponent<T>(entity);
}

template<typename T>
void ECSystem::removeComponent(Entity entity) {
  m_componentManager.removeComponent<T>(entity);
}

template<typename T>
ComponentData<T>& ECSystem::getComponentData() {
  return m_componentManager.getComponentData<T>();
}