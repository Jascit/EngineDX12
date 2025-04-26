#include <Include/Engine/Core/Systems/ECS/ECSystem.h>

bool ECSystem::initialize() {
  // Optional: Startlogik
  return true;
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