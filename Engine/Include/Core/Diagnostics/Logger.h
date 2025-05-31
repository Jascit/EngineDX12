#pragma once
#include <Core/Memory/TrackingAllocator.h>
#include <Core/Memory/STLAllocator.h>
#include <thread>
#include <mutex>
#include <vector>
#include <string>
#include <atomic>
#include <ostream>
#include <fstream>
#include <Windows.h>
#include <list>
//TODO: тоді я можу добавити писачів, які будуть пушити, і якщо ніхто не буде писати в цей момент то можна обробити останній елемент, писачі будуть опріділятись через атомарну одиницю, для швидкодії
enum class LogLevelTag : uint8_t
{
  Logs = 0,
  Debug,
  Memory
};

enum LogLevel {
  Info,
  Warning,
  Error,
  Critical
};
class Logger
{
public:
  ~Logger();
  //Logging informational messages.
  void logInfo(const std::string& message);
  // Logging warnings.
  void logWarning(const std::string& message);
  // Logging errors.
  void logError(const std::string& message);
  // Logging critical errors.
  void logCriticalError(const std::string& message);
  // Retrieve all stored logs.
  inline const char* GetLogs() {
    return m_buffer.data();
  }
  // Clear the log buffer and file
  void clearLogs();
  
  void waitForEnd();
private:
  // Write buffered logs to file.
  void flush();
  // Set the logging level (Info, Warning, Error).
  void addLogLevel(LogLevel level);
  // Add string in buffer.
  void addDescription(std::string* desc);
  // Clear the log buffer.
  void clearBuffer();

  std::pair<LogLevel, std::string*>* constructPair(LogLevel level, const std::string& message);
  void deconstructPair(std::pair<LogLevel, std::string*>* pair);
  void threadCycle();
  void push(std::pair<LogLevel, std::string*>* pair);
  void work();

private:
  std::string m_buffer;
  size_t _currentSize;
  std::mutex _mtx;
  std::atomic<uint16_t> _queueSize;
  std::atomic<uint16_t> _writerCount;
  std::thread _workerThread;
  std::list<std::pair<LogLevel, std::string*>*> _queue;


  std::condition_variable _cv;

  const char* _path;
  std::ofstream _file;
  bool _shouldStop;


public:
  Logger& operator=(const Logger&) = delete;
  Logger(const Logger&) = delete;
  inline static Logger& Get() {
    static Logger instance;
    return instance;
  }
private:
  Logger();

};

