#pragma once
#include <Include/Engine/Core/Interfaces/ISystem.h>
class IGC : ISystem {
  // Marks reachable objects from root references
  virtual void Mark() = 0;

  // Sweeps and frees unmarked objects
  virtual void Sweep() = 0;

  // Runs a full GC cycle (mark + sweep)
  virtual void CollectGarbage() = 0;

  // Returns statistics about reclaimed memory
  virtual size_t GetReclaimedMemory() const = 0;

  // Triggers the garbage collector
  virtual void runGC() = 0;
};