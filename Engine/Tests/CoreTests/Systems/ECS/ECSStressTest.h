#pragma once
#include <iostream>
#include <chrono>
#include <vector>
#include <cassert>
#include <Include/Engine/Core/Systems/ECS/ECSystem.h>
#include "PositionComponent.h"
#include <Include/Engine/Utils/WinInclude.h>

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
      PositionComponent* comp = ecs.getComponent<PositionComponent>(entities[i]);
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


  const auto& activeEntities = ecs.getActiveEntities();
  Logger::Get().logInfo("Active entities count: " + std::to_string(activeEntities.size()));

  Logger::Get().logInfo("Stress test completed successfully.");
}