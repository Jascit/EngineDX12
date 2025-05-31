#include <Core/Diagnostics/Logger.h>

void Logger::waitForEnd() {
  _shouldStop = true;
  _cv.notify_all();
  if (_workerThread.joinable()) {
    _workerThread.join();
  }
}

Logger::~Logger() {
  waitForEnd();
  if (_file.is_open()) {
    _file.close();
  }
}

Logger::Logger()
  : _shouldStop(false), _currentSize{ 0 }, _writerCount {0},
  _workerThread(std::thread(&Logger::threadCycle, this)), 
  _path(PROJECT_ROOT_DIR"/Engine/out/logs/log.txt"),
  _queueSize{0} 
{
  m_buffer.reserve(65536);
}

std::pair<LogLevel, std::string*>* Logger::constructPair(LogLevel level, const std::string& message) {
  LLM_SCOPE_BYTAG(LLMTags::STD)
  std::string* str_instance = (std::string*)GMalloc->allocate(sizeof(std::string), alignof(std::string));
  std::pair<LogLevel, std::string*>* pair_instance = (std::pair<LogLevel, std::string*>*)GMalloc->allocate(sizeof(std::pair<LogLevel, std::string*>), alignof(std::pair<LogLevel, std::string*>));
  GMalloc->construct(str_instance, message.c_str());
  GMalloc->construct(pair_instance, level, str_instance);
  return pair_instance;
}

void Logger::deconstructPair(std::pair<LogLevel, std::string*>* pair) {
  LLM_SCOPE_BYTAG(LLMTags::STD)
  GMalloc->deallocate(pair->second);
  GMalloc->deallocate(pair);
}

void Logger::logInfo(const std::string& message) {
  push(constructPair(LogLevel::Info, message));
  _cv.notify_one();
}

void Logger::logWarning(const std::string& message) {
  push(constructPair(LogLevel::Warning, message));
  _cv.notify_one();
}

void Logger::logError(const std::string& message) {
  push(constructPair(LogLevel::Error, message));
  _cv.notify_one();
}

void Logger::logCriticalError(const std::string& message) {
  push(constructPair(LogLevel::Critical, message));
  _cv.notify_one();
}

void Logger::clearLogs() {
  if (_file.is_open()) {
    _file.close();
  }

  std::ofstream file(_path, std::ios::trunc);
  file << "";
  file.close();

  _file.open(_path, std::ios::app);

  _currentSize = 0;
  m_buffer[_currentSize] = '\0';
}


void Logger::addLogLevel(LogLevel level) {
  const char* instance;
  switch (level)
  {
  case Info:
    instance = "[INFO] ";
    break;
  case Warning:
    instance = "[WARNING] ";
    break;
  case Error:
    instance = "[ERROR] ";
    break;
  case Critical:
    instance = "[CRITICAL] ";
    break;
  default:
    instance = "[UNKNOWN] ";
    break;
  }
  m_buffer += instance;
}

void Logger::flush() {
  if (_file.is_open()) {
    _file << &m_buffer[_currentSize];
    _currentSize = m_buffer.size();
  }
}

void Logger::addDescription(std::string* desc) {
  m_buffer += desc->c_str();
  m_buffer += '\n';
}

void Logger::push(std::pair<LogLevel, std::string*>* pair) {
  _writerCount.fetch_add(1, std::memory_order_release);
  std::lock_guard<std::mutex> lock(_mtx);
  _queue.push_back(pair);
  _queueSize.fetch_add(1, std::memory_order_release);
  _writerCount.fetch_sub(1, std::memory_order_release);
}

void Logger::work() {
  std::pair<LogLevel, std::string*>* pair_instance = _queue.front();
  addLogLevel(pair_instance->first);
  addDescription(pair_instance->second);
  deconstructPair(pair_instance);
  _queue.pop_front();
  flush();
  _queueSize.fetch_sub(1, std::memory_order_release);
}

/*
 * queue pushed in back, but pop on the front,
 * that can be used to synchronise the push and pull
 * actions, but size in queue(two way List) will have
 * unexpected behavior(not the problem, when there is no dependencies with it in STL)
 * TODO: that's better to do my own linked List
 *
 * [poped Nodes | synch Node | pushed Nodes]
*/

void Logger::threadCycle() {
  std::unique_lock<std::mutex> lock(_mtx);
  while (!_shouldStop)
  {
    _cv.wait(lock, [&] {
      return _queueSize.load(std::memory_order_acquire) > 1 || _shouldStop;
      });
    _file.open(_path, std::ios::app);
    while (_queueSize.load(std::memory_order_acquire) > 1)
    {
      work();
    }
    if (_writerCount.load(std::memory_order_acquire) == 0 && _queueSize.load(std::memory_order_acquire))
    {
      work(); // write last Node
    }
    _file.close();
  }
  while (_queueSize.load(std::memory_order_acquire) || _writerCount.load(std::memory_order_acquire)) { // make sure to unload all logs
    if (_queueSize.load(std::memory_order_acquire))
    {
      work();
    }
    else
    {
      std::this_thread::yield();
    }
   
  }
  
}

void Logger::clearBuffer() {
  _currentSize = 0;
  m_buffer[_currentSize] = '\0';
}
