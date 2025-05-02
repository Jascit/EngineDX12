#pragma once
#include "BaseGPUBuffer.h"
#include <Windows.h>

class RingGPUBuffer : public BaseGPUBuffer {
public:
  ~RingGPUBuffer() override;
  void update();
  void resize();
  void recreate();

};
