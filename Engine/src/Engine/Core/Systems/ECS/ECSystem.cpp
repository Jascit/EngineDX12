#include <Include/Engine/Core/Systems/ECS/ECSystem.h>

void ECSystem::initialize() {
  // Optional: Startlogik
}

void ECSystem::update() {
 
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