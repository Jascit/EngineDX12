#pragma once
#include <thread>
#include <mutex>
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
  std::mutex _mtx;
};

