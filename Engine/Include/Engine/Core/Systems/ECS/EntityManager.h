#pragma once
#include <vector>
#include <Include/Engine/Utils/WinInclude.h>
#include <Include/Engine/Core/Memory/STLAllocator.h>
//TODO: 0 refactor
using Entity = UINT;
const Entity INVALID_ENTITY = std::numeric_limits<UINT>::max();
class EntityManager {
public:   
  EntityManager();
  Entity addEntity();
  void removeEntity(Entity entity);
  const tracked_vector<Entity>& getActiveEntities() const;
private:
  tracked_vector<Entity> m_activeEntities;
  tracked_vector<Entity> m_freeEntities;
  Entity m_nextEntity;
};

