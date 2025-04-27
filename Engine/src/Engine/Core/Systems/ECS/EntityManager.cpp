#include <Include/Engine/Core/Systems/ECS/EntityManager.h>
EntityManager::EntityManager() : m_nextEntity(0) {}

Entity EntityManager::addEntity(){
	LockGuard<CriticalSection> lock(_cs);
	if (!m_freeEntities.empty())
	{
		Entity entity = m_freeEntities.back();
		m_activeEntities[entity - 1] = entity;
		m_freeEntities.pop_back();
		return entity;
	}
	m_activeEntities.push_back(m_nextEntity);
	return m_nextEntity++;
}

void EntityManager::removeEntity(Entity entity){
	LockGuard<CriticalSection> lock(_cs);
	if (entity > 0 && entity <= m_activeEntities.size() && m_activeEntities[entity - 1] != INVALID_ENTITY) {
		m_freeEntities.push_back(entity);
		m_activeEntities[entity - 1] = INVALID_ENTITY;
	}
	else {
		Logger::Get().logError("Invalid Entity ID: " + std::to_string(entity));
	}
}

const tracked_vector<Entity>& EntityManager::GetActiveEntities() const{
	return m_activeEntities;
}