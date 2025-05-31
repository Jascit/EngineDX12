#pragma once
#include "BaseGPUBuffer.hpp"
#include <Windows.hpp>

class RingGPUBuffer : public BaseGPUBuffer {
public:
  ~RingGPUBuffer() override;
  void update();
  void resize();
  void recreate();

};
