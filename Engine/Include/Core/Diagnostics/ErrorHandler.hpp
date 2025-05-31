#pragma once
#include "logger.hpp"
#include <functional>
#include <chrono>
#include <Windows.h>

class ErrorHandler
{
public:

  // Handle standard exceptions.
  void CatchException(const std::exception& e, const std::string& errorMsg);

  // Handle DirectX errors.
  void CatchHRESULT(HRESULT hr, const std::string& context);

  // Set a custom error handler.
  void SetCustomErrorHandler(std::function<void(const std::string&)>);

  //Retrieve the last error.
  void GetHandlerLastError();

  //Clear the list of errors.
  void ClearErrors();

  //Handle critical crashes (e.g., SIGSEGV).
  void CrashHandler(int signal);

  //Custom assert implementation.
  void Assert(bool condition, const std::string& message);
private:
  // Send error to Logger.
  void ReportError(const std::string& errorMessage);

private:
  std::function<void(const std::string&)> customErrorHandler;
  std::vector<std::string> errorMessages;
public:
  ErrorHandler(const ErrorHandler&) = delete;
  ErrorHandler& operator=(const ErrorHandler&) = delete;
  inline static ErrorHandler& Get() {
    static ErrorHandler instance;
    return instance;
  }
private:
  ErrorHandler() = default;
};

