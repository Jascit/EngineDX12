#pragma once
#include <Include/Engine/Core/Interfaces/ISystem.h>
#include <Include/Engine/Utils/WinInclude.h>
#include <vector>
#include <chrono>
#include <functional>
#include "ScheduleManager.h"

class TimeSystem {
public:
  using UpdateCallback = std::function<void(float)>;
  using CallbackID = UINT;
  TimeSystem() : m_deltaTime(0.f), m_fixedDelta(1.f / 60.f), m_timeScale(1.f), m_totalTime(0.f),
                 lastFrameTime(std::chrono::high_resolution_clock().now()), _nextID(0) {};
  void startFrame();
  void endFrame();

  // TODO: 3, relocate functions in TimeManager, not sure
  void update(); 
  CallbackID registerCallback(UpdateCallback func);
  void unregisterCallback(CallbackID id);
  //
  
  float GetDeltaTime() const { return m_deltaTime; }
  float GetTotalTime() const { return m_totalTime; }
  float GetFixedDelta() const { return m_fixedDelta; }
  float GetTimeScale() const { return m_timeScale; }
  void SetTimeScale(float scale) { m_timeScale = scale; }


private:
  std::chrono::high_resolution_clock::time_point lastFrameTime;
  float m_deltaTime;
  float m_totalTime;
  float m_fixedDelta;
  
  float m_timeScale;
  std::unordered_map<CallbackID, UINT> _callbacksIndices;
  std::vector<CallbackID> _lookupTable;
  std::vector<CallbackID> _freeIDs;
  std::vector<UpdateCallback> _callbacks;
  CallbackID _nextID;
};