#pragma once
//TODO: MemoryManagmentSystem, 1
#include <Include/Engine/Core/Memory/TrackingAllocator.h>
#include <Include/Engine/Core/Interfaces/ISystem.h>
class MemoryManagmentSystem : public ISystem{
public:
  void update() override;
  bool initialize() override;
  void shutdown() override;
private:

};