#include <Include/Engine/Core/Systems/ECS/ECSystem.h>

void ECSystem::initialize() {
  // Optional: Startlogik
}

void ECSystem::update() {
  // Beispiel: Iteration ?ber Entit?ten mit bestimmten Komponenten
  for (Entity entity : m_entityManager.getActiveEntities()) {
    if (hasComponent<TransformComponent>(entity) &&
      hasComponent<PhysicsComponent>(entity)) {

      auto* transform = getComponent<TransformComponent>(entity);
      auto* physics = getComponent<PhysicsComponent>(entity);

      // Beispiel-Logik: Position aktualisieren
      transform->position += physics->velocity;
    }
  }
}

void ECSystem::shutdown() {
  // Optional: Clean-up
}

Entity ECSystem::createEntity() {
  return m_entityManager.addEntity();
}

void ECSystem::removeEntity(Entity entity) {
  m_entityManager.removeEntity(entity);
}

const std::vector<Entity>& ECSystem::getActiveEntities() const {
  return m_entityManager.getActiveEntities();
}