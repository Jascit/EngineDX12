#include <Engine/Core/Interfaces/Systems/Time/BaseTickSystem.h>
BaseTickSystem::CallbackID BaseTickSystem::registerCallback(UpdateCallback func){
  CallbackID id = 0;
  if (!_freeIDs.empty()) {
    id = _freeIDs.back();
    _freeIDs.pop_back();
  }
  else {
    id = _nextID++;
  }

  _callbacksIndices[id] = _callbacks.size();
  _lookupTable.push_back(id);
  _callbacks.push_back(std::move(func));
  return id;
}

void BaseTickSystem::unregisterCallback(CallbackID id){
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
