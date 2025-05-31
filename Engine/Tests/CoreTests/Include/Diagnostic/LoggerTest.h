#include <Core/Diagnostics/Logger.h>
#include <iostream>

void loggerMultithreadedTest()
{
  const int numWriterThreads = 1;
  const int messagesPerThread = 2000;
  const int totalMessages = numWriterThreads * messagesPerThread;

  std::vector<std::thread> writerThreads;

  // Атомарні змінні для підрахунку часу (в мікросекундах) додавання пари в чергу
  std::atomic<double> pushTimeSum(0);

  // Отримуємо глобальний Logger
  Logger& logger = Logger::Get();

  // Засічемо загальний час роботи
  auto testStart = std::chrono::high_resolution_clock::now();

  // Створюємо потоки, що записують логи
  for (int i = 0; i < numWriterThreads; ++i) {
    writerThreads.emplace_back([i, messagesPerThread, &logger, &pushTimeSum]() {
      for (int j = 0; j < messagesPerThread; ++j) {

        // Засікаємо час виконання push (виклик logger.logInfo)
        auto pushStart = std::chrono::high_resolution_clock::now();
        Logger::Get().logInfo("Message " + std::to_string(j) + " from thread " + std::to_string(i));
        auto pushEnd = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double, std::micro> pushDuration = pushEnd - pushStart;
        pushTimeSum.fetch_add(pushDuration.count(), std::memory_order_relaxed);

        // Коротка пауза для імітації навантаження.
      }
      });
  }

  // Очікуємо, поки всі потоки завершаться
  for (auto& t : writerThreads) {
    if (t.joinable()) {
      t.join();
    }
  }

  // Засікаємо кінець загального часу
  auto testEnd = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> totalTestDuration = testEnd - testStart;

  // Обчислюємо середній час push-а (в мікросекундах) для одного повідомлення
  double avgPushTime = pushTimeSum.load(std::memory_order_relaxed) / totalMessages;

  std::cout << "Test complete. Check log file (log.txt) and internal buffer (first char: '"
    << logger.GetLogs() << "')\n";
  std::cout << "Average push time per log (in microseconds): " << avgPushTime << "\n";
  std::cout << "Total test duration (in milliseconds): " << totalTestDuration.count() << "\n";
}
