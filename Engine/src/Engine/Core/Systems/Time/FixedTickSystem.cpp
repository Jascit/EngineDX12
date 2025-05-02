#include <Engine/Core/Systems/Time/FixedTickSystem.h>

void FixedTickSystem::update(float fixedDeltaTime) {
  if (!_callbacks.empty())
  {
    for (auto& elem : _callbacks)
    {
      elem(fixedDeltaTime);
    }
  }
}
