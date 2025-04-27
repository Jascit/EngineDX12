#include <iostream>
#include <chrono>
#include <vector>
#include <cassert>
#include <thread>
#include <functional>
#include <Include/Engine/Core/Systems/ECS/ECSystem.h>
#include "PositionComponent.h"
#include <Include/Engine/Utils/WinInclude.h>

// Функція для отримання кількості потоків
unsigned int getThreadCount() {
  unsigned int cnt = std::thread::hardware_concurrency();
  return (cnt == 0) ? 4 : cnt;
}

void stressTestECSystem_MT() {
  ECSystem ecs; // Припускаємо, що ECSystem є потокобезпечним
  const size_t numEntities = 100000;
  std::vector<Entity> entities(numEntities);

  unsigned int numThreads = getThreadCount();
  std::vector<std::thread> threads;
  size_t chunkSize = numEntities / numThreads;

  // Створення ентиті паралельно
  auto start = std::chrono::high_resolution_clock::now();
  for (unsigned int t = 0; t < numThreads; t++) {
    size_t begin = t * chunkSize;
    size_t end = (t == numThreads - 1) ? numEntities : begin + chunkSize;
    threads.emplace_back([begin, end, &ecs, &entities]() {
      for (size_t i = begin; i < end; i++) {
        entities[i] = ecs.createEntity();
      }
      });
  }
  for (auto& th : threads) {
    th.join();
  }
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> durationCreate = end - start;
  Logger::Get().logInfo("Created " + std::to_string(numEntities) + " entities in " +
    std::to_string(durationCreate.count()) + " seconds.");

  // Паралельне додавання компонента PositionComponent (для i % 2 == 0)
  threads.clear();
  start = std::chrono::high_resolution_clock::now();
  for (unsigned int t = 0; t < numThreads; t++) {
    size_t begin = t * chunkSize;
    size_t end = (t == numThreads - 1) ? numEntities : begin + chunkSize;
    threads.emplace_back([begin, end, &ecs, &entities]() {
      for (size_t i = begin; i < end; i++) {
        if (i % 2 == 0) {
          PositionComponent pos(static_cast<int>(i), static_cast<int>(i + 1));
          ecs.addComponent<PositionComponent>(entities[i], pos);
        }
      }
      });
  }
  for (auto& th : threads) {
    th.join();
  }
  end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> durationAddComp = end - start;
  Logger::Get().logInfo("Added PositionComponent to " + std::to_string(numEntities / 2) +
    " entities in " + std::to_string(durationAddComp.count()) + " seconds.");

  // Паралельна перевірка компонентів
  threads.clear();
  start = std::chrono::high_resolution_clock::now();
  for (unsigned int t = 0; t < numThreads; t++) {
    size_t begin = t * chunkSize;
    size_t end = (t == numThreads - 1) ? numEntities : begin + chunkSize;
    threads.emplace_back([begin, end, &ecs, &entities]() {
      for (size_t i = begin; i < end; i++) {
        if (i % 2 == 0) {
          assert(ecs.hasComponent<PositionComponent>(entities[i]));
          PositionComponent* comp = ecs.GetComponent<PositionComponent>(entities[i]);
          assert(comp);
        }
        else {
          assert(!ecs.hasComponent<PositionComponent>(entities[i]));
        }
      }
      });
  }
  for (auto& th : threads) {
    th.join();
  }
  end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> durationCheckComp = end - start;
  Logger::Get().logInfo("Verified components for all entities in " +
    std::to_string(durationCheckComp.count()) + " seconds.");

  // Паралельне видалення компонентів (лише для парних індексів)
  threads.clear();
  start = std::chrono::high_resolution_clock::now();
  for (unsigned int t = 0; t < numThreads; t++) {
    size_t begin = t * chunkSize;
    size_t end = (t == numThreads - 1) ? numEntities : begin + chunkSize;
    threads.emplace_back([begin, end, &ecs, &entities]() {
      for (size_t i = begin; i < end; i++) {
        if (i % 2 == 0) {
          ecs.removeComponent<PositionComponent>(entities[i]);
        }
      }
      });
  }
  for (auto& th : threads) {
    th.join();
  }
  end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> durationRemoveComp = end - start;
  Logger::Get().logInfo("Removed components from " + std::to_string(numEntities / 2) +
    " entities in " + std::to_string(durationRemoveComp.count()) + " seconds.");

  // Паралельна перевірка видалення компонентів
  threads.clear();
  start = std::chrono::high_resolution_clock::now();
  for (unsigned int t = 0; t < numThreads; t++) {
    size_t begin = t * chunkSize;
    size_t end = (t == numThreads - 1) ? numEntities : begin + chunkSize;
    threads.emplace_back([begin, end, &ecs, &entities]() {
      for (size_t i = begin; i < end; i++) {
        if (i % 2 == 0) {
          assert(!ecs.hasComponent<PositionComponent>(entities[i]));
        }
      }
      });
  }
  for (auto& th : threads) {
    th.join();
  }
  end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> durationCheckRemoval = end - start;
  Logger::Get().logInfo("Verified removal for " + std::to_string(numEntities / 2) +
    " entities in " + std::to_string(durationCheckRemoval.count()) + " seconds.");

  // Паралельне видалення кожної 10-ої ентиті
  threads.clear();
  size_t numRemovals = numEntities / 10;
  size_t removalChunk = numRemovals / numThreads;
  start = std::chrono::high_resolution_clock::now();
  for (unsigned int t = 0; t < numThreads; t++) {
    size_t beginRemoval = t * removalChunk;
    size_t endRemoval = (t == numThreads - 1) ? numRemovals : beginRemoval + removalChunk;
    threads.emplace_back([beginRemoval, endRemoval, &ecs, &entities]() {
      for (size_t i = beginRemoval; i < endRemoval; i++) {
        size_t idx = i * 10 + 1;
        if (idx < entities.size())
          ecs.removeEntity(entities[idx]);
      }
      });
  }
  for (auto& th : threads) {
    th.join();
  }
  end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> durationRemoveEntity = end - start;
  Logger::Get().logInfo("Removed every 10th entity in " +
    std::to_string(durationRemoveEntity.count()) + " seconds.");

  const auto& activeEntities = ecs.GetActiveEntities();
  Logger::Get().logInfo("Active entities count: " + std::to_string(activeEntities.size()));
  Logger::Get().logInfo("Stress test completed successfully.");
}

int main() {
  stressTestECSystem_MT();
  return 0;
}
