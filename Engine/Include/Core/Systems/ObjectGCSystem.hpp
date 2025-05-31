// TODO: GCSystem 1, related to Memory Management System
/**
 * @class GCSystem
 * @brief Abstract base class for implementing a custom garbage collection system.
 *
 * The GCSystem is a core component of the memory management architecture.
 * It monitors memory usage, determines when to trigger garbage collection,
 * and provides hooks for registering, marking, and sweeping memory-managed objects.
 *
 * Responsibilities:
 * - Monitor available memory and determine when GC should run
 * - Register and unregister objects for tracking
 * - Perform mark and sweep phases of garbage collection
 * - Run collection during idle time
 * - Optimize memory usage and monitor GC performance
 *
 * This class should be inherited and extended to provide platform-specific
 * or engine-specific garbage collection behavior.
 */
#pragma once
#include <Core/Interfaces/IGC.hpp>

class ObjectGCSystem : IGC {
public:
  // Returns the amount of available memory
  size_t getAvailableMemory();

  // Checks if GC should run based on current memory
  bool shouldRunGC() { return getAvailableMemory() < memoryThreshold; }

  // Optimizes memory usage, e.g., adjusts thresholds
  void optimizeMemoryUsage();

  // Monitors the efficiency of GC
  void monitorGCEfficiency();

  // Runs GC during application idle time
  void collectGarbageDuringIdleTime();

  // Registers an object for tracking
  void RegisterObject();

  // Unregisters an object from GC tracking
  void UnregisterObject();

  //=== Virtuals ===
  virtual size_t GetReclaimedMemory() const override;
  virtual void Mark() override;
  virtual void Sweep() override;
  virtual void CollectGarbage() override;
  virtual void runGC() override;

private:
  size_t memoryThreshold = 1024 * 1024 * 50; // GC trigger threshold in bytes
};
