#pragma once
#include <vector>
#include <Include/Engine/Utils/WinInclude.h>
//TODO:
using Entity = size_t;
const Entity INVALID_ENTITY = std::numeric_limits<Entity>::max();
class EntityManager {
public:   
  EntityManager();
  Entity addEntity();
  void removeEntity(Entity entity);
private:
  std::vector<Entity> m_activeEntities;
  std::vector<Entity> m_freeEntities;
  Entity m_nextEntity;
};