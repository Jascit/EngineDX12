#pragma once
class IGameObject {
public:
  virtual void update(float deltaTime) = 0;
  virtual void render() = 0;
};
