#include <Include/Engine/Core/Memory/GMalloc.h>

void* GMalloc::realloc(void* block, size_t size){
  void* newBlock = ::realloc(block, size);
  if (!newBlock)
  {
    //TODO: create class GrabageCollector
    // GarbageCollector::clear()
    Logger::Get().logError("Memory reallocation failed. Size: " + std::to_string(size));
  }
  return newBlock;
}

void* GMalloc::malloc(size_t size){
  void* block = ::malloc(size);
  if (!block)
  {
    Logger::Get().logError("Memory allocation failed. Size: " + std::to_string(size));
  }
  return block;
}

void GMalloc::free(void* block){
  ::free(block); 
}
