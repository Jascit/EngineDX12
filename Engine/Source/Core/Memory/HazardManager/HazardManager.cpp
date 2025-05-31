//#include <Core/Memory/HazardManager/HazardManager.hpp>
//
///*
// * @brief  Global hazard-pointers manager, using to delete the pointer in multi-threading environment
//*/
//HazardPointerManager GHManager;
//
///*
//* @brief register hazard pointer for thread
//* @return returns an index of hazard in array, which is controled by Manager
//*/
//uint32_t* HazardPointerManager::registerThread() {
//
//}
//
//void HazardPointerManager::unregisterThread() {
//
//}
//
// /*
// * @brief add or delete the pointer, based on current usage in Threads
// * @param ptr pointer to delete.
// */
//void HazardPointerManager::retirePointer(void* ptr) {
//  if (isPointerHazard(ptr)) {
//    GMalloc->deallocate(ptr);
//    return;
//  }
//
//  uint32_t index = m_retired._size.fetch_add(1, std::memory_order_release);
//  uint32_t currentCapacity = m_retired._capacity.load(std::memory_order_acquire);
//  if (index >= currentCapacity * 0.75)
//  {
//    resizeRetiredPointerList(currentCapacity, index * 2);
//  }
//  m_retiredPointersSync._mutex.lock_shared();
//  m_retiredPointersSync._writerReaderCounter.fetch_add(1, std::memory_order_release);
//  m_retired._pointerArray[index].store(ptr, std::memory_order_release);
//  m_retiredPointersSync._writerReaderCounter.fetch_sub(1, std::memory_order_release);
//  m_retiredPointersSync._writerReaderCounter.notify_one();
//  m_retiredPointersSync._mutex.unlock_shared();
//  bool expected = false;
//  if (m_isCollecting.compare_exchange_strong(expected, true))
//  {
//    collectRetiredPointers();
//  }
//}
///*
//* @brief Private manager's function for lock-free resizing. If two threads go in, only one will resize array, another will just return to work
//* @param expectedCapacity current capacity, used for compare_exhange_strong()
//* @param desiredCapacity future capacity, used for compare_exhange_strong()
//*/
//void HazardPointerManager::resizeRetiredPointerList(uint32_t expectedCapacity, uint32_t desiredCapacity) {
//  if (m_retired._capacity.compare_exchange_strong(expectedCapacity, desiredCapacity)) {
//    m_retiredPointersSync._mutex.lock();
//    m_retiredPointersSync._writerReaderCounter.wait(0, std::memory_order_acquire);
//    m_retired._pointerArray.resize(desiredCapacity);
//    m_retiredPointersSync._mutex.unlock();
//  }
//}
//
///*
//* @brief Private manager's function for lock-free resizing. If two threads go in, only one will resize array, another will just return to work
//* @param expectedCapacity current capacity, used for compare_exhange_strong()
//* @param desiredCapacity future capacity, used for compare_exhange_strong()
//*/
//void HazardPointerManager::resizeHazardPointers(uint32_t expectedSize, uint32_t desiredSize) {
//
//}
///*
//* @brief Just delete retired pointers from list, if they aren't being used right now by Threads
//*/
//void HazardPointerManager::collectRetiredPointers() {
//  m_retiredPointersSync._mutex.lock_shared();
//  for (uint32_t i = 0; i < m_retired._size.load(std::memory_order_acquire); i++)
//  {
//    void* ptr = m_retired._pointerArray[i].load(std::memory_order_acquire);
//    if (ptr == nullptr)
//    {
//      continue;
//    }
//    // ����: ��������� ����� �� ��������� ����������� �������� ����� �� ���� ����������
//    //       (������� ����������)
//    // �������� 1:
//    //    ��������� ����� ���� ��������� �� �������:
//    //      ��� ������� �����, ����� ����� ���� �������� ���� �������� � ��������� ������, 
//    //      � ������ �� �� ���� ��������� � ���� �������������, + �� ������ �����������
//    //      ������� ���� �������, ��� ��� ������� �� �� ���� �� ���� �������:
//    //        1 ��� ������� ����� �������� ������ ��������, �� ����� ��������� ���� ��������
//    //          ������� � �������� ������, �� �������� �� ������, ����:
//    //            ���� ������� ������ ��� �������� � ������� ��� ���������
//    //        2 ��� ������� �����, ������� ���� �� ����� ���� �������, ����������� � ����
//    //          ������� � ���� �������� ������ � ����� ��� ���, ����� ��� ������ ������� 
//    //          � ��������� ��������� �������, ����:
//    //            �������� ���������� �������� �� �������, ���� �� � ��������, �� ���� ���
//    //        3 
//    // ����: ��������� ����� �� ��������� �� �����
//    // �������� 1: 
//    //    ������������ � ������ ��������, ����� ���� �������� � ����� �����, ���� �������
//    //    ���� ����:
//    //      1. ��� 
//    //
//
//    //copy, if expected -> exchange
//    if (isPointerHazard(ptr)) {
//      GMalloc->deallocate(ptr);
//    }
//  }
//  m_retiredPointersSync._mutex.unlock_shared();
//  m_isCollecting.store(false, std::memory_order_release);
//}
///*
//* @brief check if the pointer safe to delete or not
//* @param ptr pointer for checking
//* @return false if pointer is safe to delete
//* @return true if pointer is NOT safe to delete
//*/
//inline bool HazardPointerManager::isPointerHazard(void* ptr){
//
//}
