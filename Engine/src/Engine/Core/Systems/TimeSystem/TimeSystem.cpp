#include <Include/Engine/Core/Systems/TimeSystem/TimeSystem.h>

void TimeSystem::update(){
  auto now = std::chrono::high_resolution_clock().now();
  m_deltaTime = std::chrono::duration<float>(now - lastFrameTime).count();
  lastFrameTime = now;

  m_totalTime += m_deltaTime;
}

TimeSystem::CallbackID TimeSystem::registerCallback(UpdateCallback func) {
  CallbackID id = 0;
  if (!_freeIDs.empty()) {
    id = _freeIDs.back();
    _freeIDs.pop_back();
  }
  else {
    id = _nextID++;
  }
  // Speichere den Index des neuen Callbacks.
  _callbacksIndices[id] = _callbacks.size();
  _lookupTable.push_back(id);
  _callbacks.push_back(std::move(func));
  return id;
}

void TimeSystem::unregisterCallback(CallbackID id) {
  auto it = _callbacksIndices.find(id);
  if (it == _callbacksIndices.end()) {
    return;
  }

  size_t indexToDestroy = it->second;
  size_t lastIndex = _callbacks.size() - 1;

  _freeIDs.push_back(id);

  if (indexToDestroy != lastIndex) {
    _callbacks[indexToDestroy] = std::move(_callbacks[lastIndex]);
    CallbackID swappedID = _lookupTable[lastIndex];
    _lookupTable[indexToDestroy] = swappedID;
    _callbacksIndices[swappedID] = indexToDestroy;
  }

  _callbacks.pop_back();
  _lookupTable.pop_back();
  _callbacksIndices.erase(it);
}