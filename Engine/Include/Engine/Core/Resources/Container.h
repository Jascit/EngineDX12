#include <Include/Engine/Utils/WinInclude.h>
template<typename T>
class Container {
public:
  Container() : _size(0), _data(nullptr) {}

  explicit Container(size_t size) : _size(size), _data(nullptr) {
    initialize(_size);
  }

  Container(const void* data, size_t size) : _size(size), _data(nullptr) {
    initialize(_size);
    if (_data) {
      memcpy(_data, data, _size);
    }
  }

  Container(const Container&) = delete;
  Container& operator=(const Container&) = delete;

  Container(Container&& other) noexcept
    : _size(other._size), _data(other._data) {
    other._size = 0;
    other._data = nullptr;
  }

  Container& operator=(Container&& other) noexcept {
    if (this == &other) return *this;

    reset();
    _size = other._size;
    _data = other._data;
    other._size = 0;
    other._data = nullptr;
    return *this;
  }

  ~Container() {
    reset();
  }

  inline size_t GetSize() const { return _size; }
  inline void* GetData() const { return _data; }

  void reset() {
    if (_data) {
      free(_data);
      _data = nullptr;
      _size = 0;
    }
  }

  bool initialize(size_t size) {
    if(!_data) {
      _size = size;
      _data = malloc(_size);
      if (!_data) {
        Logger::Get().logError("Failed to malloc data");
        return false;
      }
      memset(_data, 0, _size);
      return true;
    }
    Logger::Get().logError("Data is still in use, cannot be initialized");
    return false;
  }

  void copyData(void* data, size_t size) {
    if (_size < size)
    {
      reset();
      initialize(size);
    }
    memcpy(_data, data, _size);
  }
  void addData(void* data, size_t size, size_t offset) {
    if (_size < size + offset)
    {
      Logger::Get().logError("Not enough CPU buffer space");
      return;
    }
    memcpy(_data, data, _size);
  }

private:
  size_t _size;
  void* _data;
};
