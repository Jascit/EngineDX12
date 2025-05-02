#pragma once
#include <iostream>
#include <chrono>
#include <vector>
#include <cstdlib>
#include <new>
#include <cassert>
#include <Engine/Core/Memory/TrackingAllocator.h>
#include <I>
#include <Engine/Core/Diagnostics/Logger.h>
#include <Engine/Core/Threading/thread.h>
#include <Engine/Utils/WinInclude.h>
#include <mutex>

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

void test() {
  std::atomic<double> gmalloc = 0;
  std::atomic<double> standartnew = 0;
  std::vector<thread> m_workers;
  int tag = 0;
  std::mutex mtx;
  for (size_t i = 0; i < 5; i++)
  {
    m_workers.push_back(thread([&](){
      int _currentTag;
      {
        std::lock_guard<std::mutex> lock(mtx);
        if (tag == static_cast<int>(LLMTags::Count))
        {
          tag = 0;
        }
        _currentTag = tag++;
      }
        LLM_SCOPE_BYTAG(LLMTags(_currentTag));
      for (size_t i = 0; i < 5; i++)
      {
        gmalloc += testGMalloc();
        standartnew += testStandardNew();
      }}));

  }

  for (int i = 0; i<5; i++)
  {
    if (m_workers[i].joinable())
    {
      m_workers[i].join();
    }
  }

  Logger::Get().logInfo("Standart new average time: " + std::to_string(standartnew.load() / 50) + " ms");
  Logger::Get().logInfo("GMalloc average time: " + std::to_string(gmalloc.load() / 50) + " ms");
  for (size_t i = 0; i < 5; i++)
  {
    switch (LLMTags(i))
    {
    case LLMTags::Assets:
      Logger::Get().logInfo("Assets: ");
      break;
    case LLMTags::Physics:
      Logger::Get().logInfo("Physics");
      break;
    case LLMTags::Rendering:
      Logger::Get().logInfo("Rendering");
      break;
    case LLMTags::Audio:
      Logger::Get().logInfo("Audio");
      break;
    case LLMTags::Unknown:
      Logger::Get().logInfo("Unknown");
      break;
    case LLMTags::Count:
      break;
    default:
      break;
    }
    auto& instance = LLMTracker::GetStats(LLMTags(i));
    Logger::Get().logInfo("Allocated " + std::to_string(instance.allocated) + " Freed " + std::to_string(instance.freed));
  }
}