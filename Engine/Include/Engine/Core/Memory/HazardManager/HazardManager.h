#pragma once
#include <Engine/Core/Memory/HazardManager/HazardPointer.h>
#include <Engine/Utils/WinInclude.h>
#include <thread>

class HazardManager {
public: 
  HazardPointer* registration();
  void unregister();
  bool retire(void* ptr);
  bool collect();

private:
  bool isHazard(void* ptr);

private:
  tracked_vector<HazardPointer> m_pointers;
  tracked_vector<uint16_t> _freeList;
  tracked_unordered_map<uint32_t, uint16_t> _threadIDToIndex;
  uint16_t _currentSize;

  tracked_vector<std::atomic<void*>> _deferredList;
};
