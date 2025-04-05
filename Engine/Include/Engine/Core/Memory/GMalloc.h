#pragma once
#include <Include/Engine/Utils/WinInclude.h>
class GMalloc {
public:
  static void* malloc(size_t size);
  static void* realloc(void* Block, size_t size);
  static void free(void* block);
};