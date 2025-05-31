#include <iostream>
#include <atomic>
#include <thread>
#include <vector>
#include <cassert>
#include <chrono>
#include <Core/Diagnostics/Logger.h>
#include <Utils/Classes/List.h> // заміни на свій хедер

constexpr int THREADS = 8;
constexpr int OPERATIONS_PER_THREAD = 10000;

List<int> list; // припустимо, твій список називається так

void pusher(int id) {
  for (int i = 0; i < OPERATIONS_PER_THREAD; ++i) {
    list.push_back(id * OPERATIONS_PER_THREAD + i);
  }
}

void popper(int& counter) {
  int local = 0;
  for (int i = 0; i < OPERATIONS_PER_THREAD; ++i) {
    auto value = list.pop_front_return(); // або pop_back_return
    if (value != 0) ++local;
  }
  counter = local;
}


int UnitTest() {
  std::vector<std::thread> pushers;
  std::vector<std::thread> poppers;
  std::vector<int> popCounts(THREADS);

  // 1. Запускаємо push
  for (int i = 0; i < THREADS; ++i)
    pushers.emplace_back(pusher, i);

  for (auto& t : pushers) t.join();

  // 2. Запускаємо pop
  for (int i = 0; i < THREADS; ++i)
    poppers.emplace_back(popper, std::ref(popCounts[i]));

  for (auto& t : poppers) t.join();

  // 3. Перевірка
  int totalPopped = 0;
  for (auto c : popCounts) totalPopped += c;

  Logger::Get().logInfo("Total pushed: " + std::to_string(THREADS * OPERATIONS_PER_THREAD));
  Logger::Get().logInfo("Total popped: " + std::to_string(totalPopped));

  assert(totalPopped <= THREADS * OPERATIONS_PER_THREAD);

  Logger::Get().logInfo("Test completed.");
  return 0;
}
