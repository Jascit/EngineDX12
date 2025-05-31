#include <Core/Diagnostics/Logger.h>
#include <iostream>

void loggerMultithreadedTest()
{
  const int numWriterThreads = 1;
  const int messagesPerThread = 2000;
  const int totalMessages = numWriterThreads * messagesPerThread;

  std::vector<std::thread> writerThreads;

  // ������� ���� ��� ��������� ���� (� ������������) ��������� ���� � �����
  std::atomic<double> pushTimeSum(0);

  // �������� ���������� Logger
  Logger& logger = Logger::Get();

  // ������� ��������� ��� ������
  auto testStart = std::chrono::high_resolution_clock::now();

  // ��������� ������, �� ��������� ����
  for (int i = 0; i < numWriterThreads; ++i) {
    writerThreads.emplace_back([i, messagesPerThread, &logger, &pushTimeSum]() {
      for (int j = 0; j < messagesPerThread; ++j) {

        // ������� ��� ��������� push (������ logger.logInfo)
        auto pushStart = std::chrono::high_resolution_clock::now();
        Logger::Get().logInfo("Message " + std::to_string(j) + " from thread " + std::to_string(i));
        auto pushEnd = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double, std::micro> pushDuration = pushEnd - pushStart;
        pushTimeSum.fetch_add(pushDuration.count(), std::memory_order_relaxed);

        // ������� ����� ��� ������� ������������.
      }
      });
  }

  // �������, ���� �� ������ �����������
  for (auto& t : writerThreads) {
    if (t.joinable()) {
      t.join();
    }
  }

  // ������� ����� ���������� ����
  auto testEnd = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> totalTestDuration = testEnd - testStart;

  // ���������� ������� ��� push-� (� ������������) ��� ������ �����������
  double avgPushTime = pushTimeSum.load(std::memory_order_relaxed) / totalMessages;

  std::cout << "Test complete. Check log file (log.txt) and internal buffer (first char: '"
    << logger.GetLogs() << "')\n";
  std::cout << "Average push time per log (in microseconds): " << avgPushTime << "\n";
  std::cout << "Total test duration (in milliseconds): " << totalTestDuration.count() << "\n";
}
