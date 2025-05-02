#include <Engine/Core/Interfaces/BaseObject.h>

std::atomic<uint32_t> BaseObject::instanceCount = 0;
int BaseObject::GetInstanceCount() {
  return instanceCount;
}
