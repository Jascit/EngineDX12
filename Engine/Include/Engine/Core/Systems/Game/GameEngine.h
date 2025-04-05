#pragma once
class GameEngine {
public:
  void initialize();
  void update();
  void render();
  void shutdown();
private:
  //TODO: systems
  //RenderingSystem renderingService;
  //PhysicsSystem physicsService;
  //InputSystem inputService;
  //AudioSystem audioService;
  //NetworkSystem networkService;
public:
  GameEngine(const GameEngine&) = delete;
  GameEngine& operator=(const GameEngine&) = delete;
  static GameEngine& Get() {
    static GameEngine instance;
    return instance;
  }
private:
  GameEngine() = default;
};