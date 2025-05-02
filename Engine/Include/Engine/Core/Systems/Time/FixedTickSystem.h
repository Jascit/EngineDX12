#pragma once
#include <Engine/Core/Interfaces/Systems/Time/BaseTickSystem.h>
#include <vector>
#include <unordered_map>
#include <functional>

class FixedTickSystem : BaseTickSystem {
public:
  void update(float fixedDeltaTime);
  ~FixedTickSystem() override {};
};
