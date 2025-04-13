#pragma once
#include <Include/Engine/Core/Systems/ECS/EntityManager.h>
#include <vector>
#include <unordered_map>
template<typename T>
class ComponentData {
public:
  void clear() {
    m_indices.clear();
    m_data.clear();
    m_entityLookup.clear();
  }

  void addComponent(Entity ID, T&& data) {
    if (m_indices.find(ID) != m_indices.end()) {
      throw std::runtime_error("Component already exists for this Entity");
    }

    m_indices[ID] = m_data.size();
    m_entityLookup.push_back(ID);
    m_data.emplace_back(std::forward<T>(data));
  }

  bool removeComponent(Entity ID) {
    auto it = m_indices.find(ID);
    if (it == m_indices.end()) return false;

    size_t indexToRemove = it->second;
    size_t lastIndex = m_data.size() - 1;

    if (indexToRemove != lastIndex)
    {
      m_data[indexToRemove] = std::move(m_data[lastIndex]);
      m_entityLookup[indexToRemove] = m_entityLookup[lastIndex];
    }
    
    m_data.pop_back();
    m_entityLookup.pop_back();
    m_indices.erase(it);

    return true;
  }

  T* getComponent(Entity ID) {
    auto it = m_indices.find(ID);
    if (it == m_indices.end()) return nullptr;
    return &m_data[it->second];
  }

  bool hasComponent(Entity ID) const {
    return m_indices.find(ID) != m_indices.end();
  }

  std::vector<std::pair<Entity, T>>& getAllComponents() { return m_data; }
  std::unordered_map<Entity, size_t>& getAllIndices() { return m_indices; }
  std::vector<Entity>& getEntityLookupTable() { return m_entityLookup; }

private:
  std::unordered_map<Entity, size_t> m_indices;
  std::vector<T> m_data;
  std::vector<Entity> m_entityLookup;
};