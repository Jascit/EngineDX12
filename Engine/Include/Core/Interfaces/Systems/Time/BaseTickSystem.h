#pragma once
#include <vector>
#include <unordered_map>
#include <functional>

class BaseTickSystem {
protected:
  using UpdateCallback = std::function<void(float)>;
  using uint = unsigned int;
  using CallbackID = uint;

public:
  CallbackID registerCallback(UpdateCallback func);
  void unregisterCallback(CallbackID id);
  virtual ~BaseTickSystem() = default;

protected:
  std::unordered_map<CallbackID, uint> _callbacksIndices;
  std::vector<CallbackID> _lookupTable;
  std::vector<CallbackID> _freeIDs;
  std::vector<UpdateCallback> _callbacks;
  CallbackID _nextID = 0;
};
