#pragma once
class Window
{
public:
private:

public:
  Window(const Window&) = delete;
  Window& operator=(const Window&) = delete;
  inline const Window& Get() {
    static Window instance;
    return instance;
  }
private:
  Window() = default;
};

