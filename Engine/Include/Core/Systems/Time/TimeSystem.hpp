#pragma once
#include <Core/Interfaces/ISystem.hpp>
#include <Utils/WinInclude.hpp>
#include <vector>
#include <chrono>
#include <functional>
#include "ScheduleManager.hpp"

class TimeSystem {
public:
  TimeSystem() : m_deltaTime(0.f), m_fixedDelta(1.f / 60.f), m_timeScale(1.f), m_totalTime(0.f),
                 lastFrameTime(std::chrono::high_resolution_clock::now()) {};

  void updateDelta();
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
};
