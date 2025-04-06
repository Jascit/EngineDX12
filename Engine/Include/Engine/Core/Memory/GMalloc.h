#pragma once
#include <Include/Engine/Utils/WinInclude.h>
class GMalloc {
public:
  static void* allocate(size_t size);
  static void* reallocate(void* Block, size_t size);
  static void deallocate(void* block);
};