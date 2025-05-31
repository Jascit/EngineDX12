#include <Core/Interfaces/BaseObject.hpp>

std::atomic<uint32_t> BaseObject::instanceCount = 0;
int BaseObject::GetInstanceCount() {
  return instanceCount;
}
