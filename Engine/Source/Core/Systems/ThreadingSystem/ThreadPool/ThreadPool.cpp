#include <Core/Systems/ThreadingSystem/ThreadPool/ThreadPool.h>

ThreadPool::ThreadPool()
  : _shouldStop(false)
{
  for (size_t i = 0; i < THREADCOUNT_FOR_THREAD_POOL; i++)
  {
    _workerThreads.push_back(std::thread(&ThreadPool::threadCycle, this));  // Add `this` to set the correct context
  }
}

void ThreadPool::addTask(ITask* task) {
  if (task != nullptr) {
    _queue.push(task);  // _queue is already thread-safe
    _cv.notify_one();   // notify_one is sufficient here to wake up one thread
  }
}

void ThreadPool::shutdown() {
  {
    std::lock_guard<std::mutex> lock(_mtx);  // Only for shutdown and flag protection
    _shouldStop = true;
  }
  _cv.notify_all();  // notify_all to make sure all threads receive the stop signal

  for (size_t i = 0; i < THREADCOUNT_FOR_THREAD_POOL; i++) {
    if (_workerThreads[i].joinable()) {
      _workerThreads[i].join();  // Wait for all threads to finish
    }
  }
}

void ThreadPool::threadCycle() {
  while (true) {
    std::unique_lock<std::mutex> lock(_mtx);

    // Wait if the queue is empty or the shutdown signal is set
    _cv.wait(lock, [&] {
      return !_queue.empty() || _shouldStop;
      });

    if (_shouldStop && _queue.empty()) {
      break;  // All threads should stop if the queue is empty and shutdown signal is set
    }

    // Process tasks while there are any in the queue
    while (!_queue.empty()) {
      ITask* task = _queue.pop();
      if (task != nullptr) {
        task->execute();
      }
    }
  }
}
