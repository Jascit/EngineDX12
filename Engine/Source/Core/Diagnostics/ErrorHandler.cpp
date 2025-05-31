#include <Core/Diagnostics/ErrorHandler.h>

void ErrorHandler::CatchException(const std::exception& e, const std::string& errorMsg) {
  std::string msg = errorMsg;
  msg.erase(msg.begin() + msg.size() - 2, msg.end());
  Logger::Get().logError("Exception caught: " + std::string(e.what()) + " - Context: " + msg);

  std::time_t now = std::time(nullptr);
  ReportError("Exception occurred at: " + std::to_string(now) + " - " + e.what());
}

void ErrorHandler::CatchHRESULT(HRESULT hr, const std::string& context) {
  auto now = std::chrono::system_clock::now();
  auto now_c = std::chrono::system_clock::to_time_t(now);
  std::tm timeinfo;
  localtime_s(&timeinfo, &now_c);

  std::stringstream timeStream;
  timeStream << std::put_time(&timeinfo, "%H:%M:%S");

  std::string timestamp = timeStream.str();

  if (SUCCEEDED(hr)) {
    Logger::Get().logInfo("[" + timestamp + "] - " + context);
  }
  else {
    wchar_t wErrorMsg[512];
    char errorMsg[512];
    DWORD dw = FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, hr, 0, wErrorMsg, sizeof(wErrorMsg) / sizeof(wchar_t), NULL);

    size_t converted = 0;
    size_t errorMsgSize = sizeof(errorMsg) / sizeof(errorMsg[0]);
    errno_t err = wcstombs_s(&converted, errorMsg, errorMsgSize, wErrorMsg, _TRUNCATE);
    if (err != 0) {
      Logger::Get().logWarning("[" + timestamp + "] - Cannot convert msg in ErrorHandler::CatchHRESULT");
      ReportError("[" + timestamp + "] - HRESULT Error Message (conversion failed): " + std::to_string(hr));
      return;
    }

    if (dw == 0) {
      DWORD lastError = GetLastError();
      ReportError("[" + timestamp + "] - Failed to format error message for HRESULT: " + std::to_string(hr) + " - Context: " + context + " - LastError: " + std::to_string(lastError));
      return;
    }
    else {
      std::string msg = errorMsg;
      msg.erase(msg.begin() + msg.size() - 2, msg.end());
      ReportError("[" + timestamp + "] - HRESULT Error Message: " + msg + " - Context: " + context);
    }
  }
}


void ErrorHandler::SetCustomErrorHandler(std::function<void(const std::string&)> customHandler) {
  customErrorHandler = customHandler;
}

void ErrorHandler::ReportError(const std::string& errorMessage) {
  if (customErrorHandler) {
    errorMessages.push_back(errorMessage);
    customErrorHandler(errorMessage);
  }
  else {
    errorMessages.push_back(errorMessage);
    Logger::Get().logError(errorMessage);
  }
}


void ErrorHandler::GetHandlerLastError() {
  if (!errorMessages.empty()) {
    Logger::Get().logInfo("Last error: " + errorMessages.back());
  }
  else {
    Logger::Get().logInfo("No errors found.");
  }
}
void ErrorHandler::ClearErrors() {
  errorMessages.clear();
  Logger::Get().logInfo("All errors have been cleared.");
}

void ErrorHandler::CrashHandler(int signal) {

  Logger::Get().logError("Crash signal received: " + std::to_string(signal));

  std::time_t now = std::time(nullptr);
  ReportError("Crash occurred at: " + std::to_string(now) + " - Signal: " + std::to_string(signal));

  exit(signal);
}


void ErrorHandler::Assert(bool condition, const std::string& message) {
  if (!condition) {
    ReportError("Assertion failed: " + message);

    abort();
  }
}
