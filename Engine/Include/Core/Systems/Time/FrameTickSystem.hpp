#pragma once
#include <Core/Interfaces/Systems/Time/BaseTickSystem.hpp>
#include <vector>
#include <unordered_map>
#include <functional>

class FrameTickSystem : BaseTickSystem {
public:
  void update(float frameDeltaTime);
  ~FrameTickSystem() override {};
};
