#pragma once
#include <Core/Interfaces/IComponentData.hpp>
#include <Core/Memory/STLAllocator.hpp>
#include <Utils/WinInclude.hpp>
#include <shared_mutex>
#include <vector>
#include <unordered_map>
//Delete O(1), find O(1), emplace O(1)
template<typename T>
class ComponentData : IComponentData {
public:
  ~ComponentData() override {};
  virtual void destroy() noexcept override {
    this->~ComponentData<T>();
    GMalloc->deallocate(this);// Unknown Tag
  }

  void clear() {
    std::unique_lock lock(_mtx);
    m_indices.clear();
    m_data.clear();
    m_entityLookup.clear();
  }

  void addComponent(Entity ID, T&& data) {
    std::unique_lock lock(_mtx);
    if (m_indices.find(ID) != m_indices.end()) {
      throw std::runtime_error("Component already exists for this Entity");
    }

    m_data.emplace_back(std::forward<T>(data));
    m_indices[ID] = _size++;
    m_entityLookup.push_back(ID);
  }

  bool removeComponent(Entity ID) {
    std::unique_lock lock(_mtx);
    auto it = m_indices.find(ID);
    if (it == m_indices.end()) return false;

    UINT indexToRemove = it->second;
    UINT lastIndex = m_data.size() - 1;

    if (indexToRemove != lastIndex)
    {
      m_data[indexToRemove] = std::move(m_data[lastIndex]);
      m_entityLookup[indexToRemove] = m_entityLookup[lastIndex];
      m_indices[m_entityLookup[lastIndex]] = indexToRemove;
    }

    m_data.pop_back();
    m_entityLookup.pop_back();
    m_indices.erase(it);
    _size--;
    return true;
  }

  T* GetComponent(Entity ID) {
    std::shared_lock<std::shared_mutex> lock(_mtx);
    auto it = m_indices.find(ID);
    if (it == m_indices.end()) return nullptr;
    return &m_data[it->second];
  }

  bool hasComponent(Entity ID) const {
    return m_indices.find(ID) != m_indices.end();
  }

  tracked_vector<T>& getAllComponents() { return m_data; }
  tracked_unordered_map<Entity, UINT>& getAllIndices() { return m_indices; }
  tracked_vector<Entity>& getEntityLookupTable() { return m_entityLookup; }

private:
  tracked_unordered_map<Entity, UINT> m_indices;
  tracked_vector<T> m_data;
  tracked_vector<Entity> m_entityLookup;
  UINT _size = 0;
  std::shared_mutex _mtx;
};
