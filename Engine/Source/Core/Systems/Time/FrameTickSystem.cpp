#include <Core/Systems/Time/FrameTickSystem.hpp>
void FrameTickSystem::update(float frameDeltaTime) {
  if (!_callbacks.empty())
  {
    for (auto& elem : _callbacks)
    {
      elem(frameDeltaTime);
    }
  }
}
