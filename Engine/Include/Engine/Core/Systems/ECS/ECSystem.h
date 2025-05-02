#pragma once
#include <Engine/Core/Interfaces/ISystem.h>
#include <Engine/Core/Systems/ECS/ComponentManager.h>
#include <Engine/Core/Systems/ECS/EntityManager.h>
// The ECSystem encapsulates the work with entities and components.
// It serves as a central interface for the Entity Component System.
class ECSystem : public ISystem {
public:
  bool initialize() override;
  void update() override;
  void shutdown() override;

  // --- Entity-Logic ---
  Entity createEntity();
  void removeEntity(Entity entity);
  const tracked_vector<Entity>& GetActiveEntities() const;

  // --- Components-Logic ---
  template<typename T>
  void addComponent(Entity entity, const T& component);

  template<typename T>
  T* GetComponent(Entity entity);

  template<typename T>
  bool hasComponent(Entity entity);

  // TODO: 2 Communication with GCS to delete unneeded components and entities
  template<typename T>
  void removeComponent(Entity entity);

  template<typename T>
  ComponentData<T>& GetComponentData();

  template<typename T>
  void clearComponents();
private:
  EntityManager m_entityManager;
  ComponentManager m_componentManager;
};

template<typename T>
void ECSystem::addComponent(Entity entity, const T& component) {
  m_componentManager.addComponent<T>(entity, component);
}

template<typename T>
T* ECSystem::GetComponent(Entity entity) {
  return m_componentManager.GetComponent<T>(entity);
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
ComponentData<T>& ECSystem::GetComponentData() {
  return *m_componentManager.GetComponentData<T>();
}
template<typename T>
void ECSystem::clearComponents() {
  GetComponentData<T>().clear();
}
