#pragma once
#include <Engine/Core/Threading/CriticalSections/CriticalSection.h>
#include <Engine/Core/Threading/Lockers/LockGuard.h>
#include <Engine/Core/Memory/STLAllocator.h>
#include <Engine/Utils/WinInclude.h>
#include <vector>
//TODO: 0 refactor
class EntityManager {
public:
  EntityManager();
  Entity addEntity();
  void removeEntity(Entity entity);
  const tracked_vector<Entity>& GetActiveEntities() const;
private:
  tracked_vector<Entity> m_activeEntities;
  tracked_vector<Entity> m_freeEntities;
  Entity m_nextEntity;
  CriticalSection _cs;
};

