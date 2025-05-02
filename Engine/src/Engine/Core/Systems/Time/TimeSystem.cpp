#include <Engine/Core/Systems/Time/TimeSystem.h>
using UpdateCallback = std::function<void(float)>;
void TimeSystem::updateDelta(){
  auto now = std::chrono::high_resolution_clock::now();
  m_deltaTime = std::chrono::duration<float>(now - lastFrameTime).count();
  lastFrameTime = now;

  m_totalTime += m_deltaTime;
}
