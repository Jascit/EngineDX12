#pragma once
#include <Include/Engine/Core/Interfaces/BaseIterator.h>
#include <Include/Engine/Core/Memory/GMalloc.h>
#include <set>
//TODO:
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
    _freeBlocks.emplace(_freeBlock);
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

  void freeBlock(void* ptr) {

  }

  void defragment() {

  }

  void reallocate() {

  }

  void resize() {

  }

  T& operator[](size_t index) {
    if (index > _blockCount*0.75)
    {
      
    }
    return;
  }
  class Iterator : BaseIterator<T, Iterator> {
    using BaseIterator = BaseIterator<T, Iterator>;
  public:
    Iterator(T* ptr) : BaseIterator(ptr) {};
    void operator++() override {
      ++m_ptr;
    }
    void operator--() override {
      --m_ptr;
    }
  };
  Iterator& end() {
    Iterator temp(*_pool + _currentBlockOffset);
    return temp;
  }
  Iterator& begin() {
    Iterator temp(*_pool);
    return temp;
  }
private:
  size_t _blockCount;
  size_t _blockSize;
  size_t _currentBlockOffset;
  T** _pool;
  std::set<_FreeBlock> _freeBlocks;
};