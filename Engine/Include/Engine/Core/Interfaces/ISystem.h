#pragma once
class ISystem {
public:
  virtual void update() = 0;
  virtual void initialize() = 0;
  virtual void shutdown() = 0;
};