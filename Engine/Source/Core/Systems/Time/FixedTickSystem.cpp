#include <Core/Systems/Time/FixedTickSystem.hpp>

void FixedTickSystem::update(float fixedDeltaTime) {
  if (!_callbacks.empty())
  {
    for (auto& elem : _callbacks)
    {
      elem(fixedDeltaTime);
    }
  }
}
