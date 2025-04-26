#pragma once
#include <iostream>
#include <chrono>
#include <vector>
#include <cstdlib>
#include <new>
#include <cassert>
#include <Include/Engine/Core/Memory/TrackingAllocator.h>  // Make sure this header includes the TrackingAllocator definition
#include <Include/Engine/Core/Diagnostics/Logger.h>

constexpr size_t NUM_ALLOCATIONS = 1000000;
constexpr size_t ALLOCATION_SIZE = 128;

double testStandardNew() {
  Logger::Get().logInfo("Performance test of standart new: " + std::to_string(NUM_ALLOCATIONS) + " allocations of " + std::to_string(ALLOCATION_SIZE) + " bytes");
  std::vector<void*> ptrs;
  ptrs.reserve(NUM_ALLOCATIONS);

  auto start = std::chrono::high_resolution_clock::now();

  for (size_t i = 0; i < NUM_ALLOCATIONS; ++i) {
    void* mem = ::operator new(ALLOCATION_SIZE);
    ptrs.push_back(mem);
  }

  for (void* p : ptrs) {
    ::operator delete(p);
  }

  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> elapsed = end - start;
  Logger::Get().logInfo("Standard new/delete time: " + std::to_string(elapsed.count()) + " ms");
  return elapsed.count();
}

double testGMalloc() {
  Logger::Get().logInfo("Performance test of GMalloc: " + std::to_string(NUM_ALLOCATIONS) + " allocations of " + std::to_string(ALLOCATION_SIZE) + " bytes");
  std::vector<void*> ptrs;
  ptrs.reserve(NUM_ALLOCATIONS);

  auto start = std::chrono::high_resolution_clock::now();

  for (size_t i = 0; i < NUM_ALLOCATIONS; ++i) {
    void* mem = GMalloc->allocate(ALLOCATION_SIZE);
    assert(mem != nullptr);
    ptrs.push_back(mem);
  }

  for (void* p : ptrs) {
    GMalloc->deallocate(p);
  }

  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> elapsed = end - start;
  Logger::Get().logInfo("GMalloc time: " + std::to_string(elapsed.count()) + " ms");
  return elapsed.count();
}
