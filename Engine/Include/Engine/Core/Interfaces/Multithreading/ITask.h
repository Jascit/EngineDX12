#pragma once
#include <atomic>
#include <string>
#include <vector>
class ITask {
public:
  virtual void execute() = 0;
  virtual bool isCompleted() = 0;
  virtual bool isVisited() = 0;
  virtual void Visited() = 0;
  virtual void addDependence(uint32_t taskID) = 0;
  virtual void addDependence(ITask* task) = 0;
  virtual std::vector<uint32_t>& GetDependencies() = 0;
  virtual uint32_t GetID() = 0;
};