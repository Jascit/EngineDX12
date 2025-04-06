#pragma once
#include <Include/Engine/Core/Interfaces/BaseIterator.h>
#include <Include/Engine/Core/Memory/GMalloc.h>
#include <set>
//TODO: 2 refactor
template<typename T>
class ObjectMemoryPool {
private:
  struct _FreeBlock {
    size_t start;
    size_t end;
  };
public:
  ObjectMemoryPool(size_t blockCount) : _blockCount(blockCount), _blockSize(sizeof(T)), _currentBlockOffset(0), _pool(nullptr) {
    T* ptr = reinterpret_cast<T*>(GMalloc::malloc(_blockCount * _blockSize));
    _FreeBlock _freeBlock = { 0, blockCount - 1 };
    _pool = ptr;
  }
  ~ObjectMemoryPool() {
    shutdown();
  };

  void shutdown() {
    if (_pool)
    {
      GMalloc::free(*_pool);
      _pool = nullptr;
    }
  }


  void reallocate() {

  }

  void resize(size_t size) {

  }

private:
  size_t _blockCount;
  size_t _blockSize;
  size_t _size;
  T* _pool;
 };