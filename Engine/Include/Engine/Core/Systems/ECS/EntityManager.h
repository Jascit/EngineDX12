#pragma once
#include <vector>
#include <Include/Engine/Utils/WinInclude.h>
//TODO: 0 refactor
using Entity = UINT;
const Entity INVALID_ENTITY = std::numeric_limits<UINT>::max();
class EntityManager {
public:   
  EntityManager();
  Entity addEntity();
  void removeEntity(Entity entity);
  const std::vector<Entity>& getActiveEntities() const;
private:
  std::vector<Entity> m_activeEntities;
  std::vector<Entity> m_freeEntities;
  Entity m_nextEntity;
};

