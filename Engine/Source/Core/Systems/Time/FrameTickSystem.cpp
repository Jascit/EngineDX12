#include <Core/Systems/Time/FrameTickSystem.h>
void FrameTickSystem::update(float frameDeltaTime) {
  if (!_callbacks.empty())
  {
    for (auto& elem : _callbacks)
    {
      elem(frameDeltaTime);
    }
  }
}
