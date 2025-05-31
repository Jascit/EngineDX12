#pragma once
#include <Core/Interfaces/Systems/Time/BaseTickSystem.hpp>
#include <vector>
#include <unordered_map>
#include <functional>

class FixedTickSystem : BaseTickSystem {
public:
  void update(float fixedDeltaTime);
  ~FixedTickSystem() override {};
};
