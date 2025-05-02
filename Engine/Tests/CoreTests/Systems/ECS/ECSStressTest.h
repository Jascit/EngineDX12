#include <iostream>
#include <chrono>
#include <vector>
#include <cassert>
#include <thread>
#include <functional>
#include <Engine/Core/Systems/ECS/ECSystem.h>
#include "PositionComponent.h"
#include "nameComponent.h"
#include "dataComponent.h"
#include "sumComponent.h"
#include <Engine/Utils/WinInclude.h>

// Функція для отримання кількості потоків
unsigned int getThreadCount() {
  unsigned int cnt = std::thread::hardware_concurrency();
  return (cnt == 0) ? 4 : cnt;
}
void stressTestECSystem_MT_MultiComponents() {
  ECSystem ecs; // Припускаємо, що ECSystem є потокобезпечною
  const size_t numEntities = 100000;
  std::vector<Entity> entities(numEntities);

  unsigned int numThreads = getThreadCount();
  std::vector<std::thread> threads;
  size_t chunkSize = numEntities / numThreads;

  // 1. Паралельне створення сутностей
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
  Logger::Get().logInfo("Created " + std::to_string(numEntities) +
    " entities in " + std::to_string(durationCreate.count()) + " seconds.");

  // 2. Паралельне додавання різних компонентів:
  // Для парних індексів додаємо PositionComponent і nameComponent,
  // для непарних – dataComponent і sumComponent.
  threads.clear();
  start = std::chrono::high_resolution_clock::now();
  for (unsigned int t = 0; t < numThreads; t++) {
    size_t begin = t * chunkSize;
    size_t end = (t == numThreads - 1) ? numEntities : begin + chunkSize;
    threads.emplace_back([begin, end, &ecs, &entities]() {
      for (size_t i = begin; i < end; i++) {
        if (i % 2 == 0) {
          // Додаємо PositionComponent та nameComponent
          PositionComponent pos(i, i + 1);
          nameComponent name("Entity_" + std::to_string(i));
          ecs.addComponent<PositionComponent>(entities[i], pos);
          ecs.addComponent<nameComponent>(entities[i], name);
        }
        else {
          // Додаємо dataComponent та sumComponent
          void* someData = reinterpret_cast<void*>(i);
          dataComponent data(someData);
          sumComponent sum(i * 10);
          ecs.addComponent<dataComponent>(entities[i], data);
          ecs.addComponent<sumComponent>(entities[i], sum);
        }
      }
      });
  }
  for (auto& th : threads) {
    th.join();
  }
  end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> durationAddComponents = end - start;
  Logger::Get().logInfo("Added different components to " + std::to_string(numEntities) +
    " entities in " + std::to_string(durationAddComponents.count()) + " seconds.");

  // 3. Паралельна перевірка наявності компонент
  threads.clear();
  start = std::chrono::high_resolution_clock::now();
  for (unsigned int t = 0; t < numThreads; t++) {
    size_t begin = t * chunkSize;
    size_t end = (t == numThreads - 1) ? numEntities : begin + chunkSize;
    threads.emplace_back([begin, end, &ecs, &entities]() {
      for (size_t i = begin; i < end; i++) {
        if (i % 2 == 0) {
          // Переконуємося, що присутні PositionComponent і nameComponent
          assert(ecs.hasComponent<PositionComponent>(entities[i]));
          assert(ecs.hasComponent<nameComponent>(entities[i]));
        }
        else {
          // Переконуємося, що присутні dataComponent і sumComponent
          assert(ecs.hasComponent<dataComponent>(entities[i]));
          assert(ecs.hasComponent<sumComponent>(entities[i]));
        }
      }
      });
  }
  for (auto& th : threads) {
    th.join();
  }
  end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> durationCheckComponents = end - start;
  Logger::Get().logInfo("Verified all components in " + std::to_string(durationCheckComponents.count()) + " seconds.");

  // 4. Паралельне видалення компонентів:
  // Для парних видаляємо PositionComponent і для непарних – dataComponent,
  // та видаляємо nameComponent і sumComponent одночасно як додаткову операцію.
  threads.clear();
  start = std::chrono::high_resolution_clock::now();
  for (unsigned int t = 0; t < numThreads; t++) {
    size_t begin = t * chunkSize;
    size_t end = (t == numThreads - 1) ? numEntities : begin + chunkSize;
    threads.emplace_back([begin, end, &ecs, &entities]() {
      for (size_t i = begin; i < end; i++) {
        if (i % 2 == 0) {
          ecs.removeComponent<PositionComponent>(entities[i]);
          ecs.removeComponent<nameComponent>(entities[i]);
        }
        else {
          ecs.removeComponent<dataComponent>(entities[i]);
          ecs.removeComponent<sumComponent>(entities[i]);
        }
      }
      });
  }
  for (auto& th : threads) {
    th.join();
  }
  end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> durationRemoveComponents = end - start;
  Logger::Get().logInfo("Removed components from all entities in " +
    std::to_string(durationRemoveComponents.count()) + " seconds.");

  // 5. Паралельна перевірка видалення компонентів
  threads.clear();
  start = std::chrono::high_resolution_clock::now();
  for (unsigned int t = 0; t < numThreads; t++) {
    size_t begin = t * chunkSize;
    size_t end = (t == numThreads - 1) ? numEntities : begin + chunkSize;
    threads.emplace_back([begin, end, &ecs, &entities]() {
      for (size_t i = begin; i < end; i++) {
        if (i % 2 == 0) {
          assert(!ecs.hasComponent<PositionComponent>(entities[i]));
          assert(!ecs.hasComponent<nameComponent>(entities[i]));
        }
        else {
          assert(!ecs.hasComponent<dataComponent>(entities[i]));
          assert(!ecs.hasComponent<sumComponent>(entities[i]));
        }
      }
      });
  }
  for (auto& th : threads) {
    th.join();
  }
  end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> durationVerifyRemoval = end - start;
  Logger::Get().logInfo("Verified removal of components in " +
    std::to_string(durationVerifyRemoval.count()) + " seconds.");

  // 6. Додатково, паралельне видалення деяких сутностей (наприклад, кожної 10-ої)
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
  Logger::Get().logInfo("Stress test completed successfully with multiple components.");
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

void stressTestECSystem() {
  ECSystem ecs;
  const size_t numEntities = 100000;
  std::vector<Entity> entities;
  entities.reserve(numEntities);

  auto start = std::chrono::high_resolution_clock::now();
  for (size_t i = 0; i < numEntities; i++) {
    Entity e = ecs.createEntity();
    entities.push_back(e);
  }
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> durationCreate = end - start;
  Logger::Get().logInfo("Created " + std::to_string(numEntities) + " entities in " + std::to_string(durationCreate.count()) + " seconds.");

  start = std::chrono::high_resolution_clock::now();
  for (size_t i = 0; i < numEntities; i++) {
    if (i % 2 == 0) {
      PositionComponent pos(i, i + 1);
      ecs.addComponent<PositionComponent>(entities[i], pos);
    }
  }
  end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> durationAddComp = end - start;
  Logger::Get().logInfo("Added PositionComponent to " + std::to_string(numEntities / 2) + " entities in " + std::to_string(durationAddComp.count()) + " seconds.");

  start = std::chrono::high_resolution_clock::now();
  for (size_t i = 0; i < numEntities; i++) {
    if (i % 2 == 0) {
      assert(ecs.hasComponent<PositionComponent>(entities[i]));
      PositionComponent* comp = ecs.GetComponent<PositionComponent>(entities[i]);
      assert(comp);
    }
    else {
      assert(!ecs.hasComponent<PositionComponent>(entities[i]));
    }
  }
  end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> durationCheckComp = end - start;
  Logger::Get().logInfo("Verified components for all entities in " + std::to_string(durationCheckComp.count()) + " seconds.");

  start = std::chrono::high_resolution_clock::now();
  for (size_t i = 0; i < numEntities; i++) {
    if (i % 2 == 0) {
      ecs.removeComponent<PositionComponent>(entities[i]);
    }
  }
  end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> durationRemoveComp = end - start;
  Logger::Get().logInfo("Removed components from " + std::to_string(numEntities / 2) + " entities in " + std::to_string(durationRemoveComp.count()) + " seconds.");

  start = std::chrono::high_resolution_clock::now();
  for (size_t i = 0; i < numEntities; i++) {
    if (i % 2 == 0) {
      assert(!ecs.hasComponent<PositionComponent>(entities[i]));
    }
  }
  end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> durationCheckRemoval = end - start;
  Logger::Get().logInfo("Verified removal for " + std::to_string(numEntities / 2) + " entities in " + std::to_string(durationCheckRemoval.count()) + " seconds.");

  start = std::chrono::high_resolution_clock::now();
  for (size_t i = 1; i < entities.size(); i += 10) {
    ecs.removeEntity(entities[i]);
  }
  end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> durationRemoveEntity = end - start;
  Logger::Get().logInfo("Removed every 10th entity in " + std::to_string(durationRemoveEntity.count()) + " seconds.");


  const auto& activeEntities = ecs.GetActiveEntities();
  Logger::Get().logInfo("Active entities count: " + std::to_string(activeEntities.size()));

  Logger::Get().logInfo("Stress test completed successfully.");
}
