#pragma once
#include <Include/Engine/Core/AbstractClasses/Systems/Time/BaseTickSystem.h>
#include <vector>
#include <unordered_map>
#include <functional>

class FrameTickSystem : BaseTickSystem {
public:
  void update(float frameDeltaTime);
  ~FrameTickSystem() override {};
};