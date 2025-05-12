#pragma once
#include <Engine/Core/Memory/HazardManager/HazardPointer.h>
#include <Engine/Utils/WinInclude.h>
#include <thread>

class HazardManager {
public: 
// hazard_pointers
  HazardPointer* registerTread();
  void unregisterThread();
  bool retire(void* ptr);
  bool collect();

private:
  bool isHazard(void* ptr);

private:
  tracked_vector<HazardPointer> m_pointers;
  tracked_vector<std::atomic<uint16_t>> _freeList;
  //RCU - delete indices
  tracked_unordered_map<uint32_t, std::atomic<uint32_t>> _threadIDToIndex;
  std::atomic<uint16_t> _currentSize;

  tracked_vector<std::atomic<void*>> _retiredList;
};
