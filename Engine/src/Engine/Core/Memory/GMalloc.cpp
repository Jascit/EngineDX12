#include <Include/Engine/Core/Memory/GMalloc.h>

void* GMalloc::reallocate(void* block, size_t newSize) {
  void* newBlock = allocate(newSize);
  if (!newBlock) {
    Logger::Get().logError("Memory reallocation failed. Size: " + std::to_string(newSize));
    return nullptr;
  }

  std::memcpy(newBlock, block, newSize);
  deallocate(block);
  return newBlock;
}

void* GMalloc::allocate(size_t size) {
  void* block = nullptr;
  try {
    block = ::operator new(size);
  }
  catch (const std::bad_alloc& e) {
    ErrorHandler::Get().CatchException(e, "Memory allocation failed. Size: " + std::to_string(size));
  }
  return block;
}

void GMalloc::deallocate(void* block) {
  try {
    delete block;
  }
  catch (...) {
    Logger::Get().logError("Error occurred while deallocating memory.");
  }
}