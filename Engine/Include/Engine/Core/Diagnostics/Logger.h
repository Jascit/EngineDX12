#pragma once
#include <vector>
#include <string>
#include <ostream>
#include <fstream>
#include <Windows.h>
#include <list>
#include <Include/Engine/Core/Threading/thread.h>
#include <Include/Engine/Core/Threading/CriticalSections/CriticalSection.h>
#include <Include/Engine/Core/Threading/ConditionalVariables/ConditionalVariableCS.h>
#include <Include/Engine/Core/Threading/Lockers/LockGuard.h>
#include <Include/Engine/Core/Threading/Lockers/CriticalSection/UniqueLockCS.h>

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
  void logInfo(const std::string message);
  // Logging warnings.
  void logWarning(const std::string message);
  // Logging errors.
  void logError(const std::string message);
  // Logging critical errors.
  void logCriticalError(const std::string message);
  //EnableConsoleOutput(bool enable) Ц ¬ключенн€ / вимкненн€ виводу в консоль. // Enable/disable console output.
  //EnableFileOutput(const std::string & filePath) Ц ¬ключенн€ логуванн€ у файл. // Enable logging to a file.
  // Retrieve all stored logs.
  inline char& GetLogs() {
    return *m_buffer;
  }
  // Clear the log buffer and file
  void clearLogs();
private:
  // Write buffered logs to file.
  void flush();
  void startWorker();
  // Set the logging level (Info, Warning, Error).
  void setLogLevel(LogLevel level);
  // Add string in buffer.
  void addLog(const std::string message);
  void writeInLogs(const std::string message, LogLevel level);
  void push(std::pair<LogLevel, std::string> pair);
  void threadCycle();
  // Clear the log buffer.
  void clearBuffer();

private:
  char m_buffer[65536];
  size_t m_currentSize;
  size_t m_lastSize;
  CriticalSection m_cs;
  CriticalSection m_csSync;
  thread m_workerThread;
  std::list<std::pair<LogLevel, std::string>> m_queue;
  ConditionalVariableCS m_condition;

  std::ofstream m_file;
  bool m_shouldStop;
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

