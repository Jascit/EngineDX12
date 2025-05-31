//#pragma once
//#include <Core/Memory/HazardManager/HazardPointer.hpp>
//#include <Utils/WinInclude.hpp>
//#include <shared_mutex>
//#include <thread>
//
//// 3. Масив з ресайзом, потоки звертаються до центрального компонента для отримання і змін хазарда, 
////    можливо через спец обєкт(+)
////    опис: простий вектор з поінтерами, можливо ще одна структура для знаходження власного хазарда, 
////          або ж знаходження хазарда по індексам в масиві які видаються при регістрації потока 
////    недоліки: час на зверення до хазарда зменшиться, но в порівнянні з іншими варіантами це припустимо
////              потрібно додаткову логіку для синхронізації між потоками в багатопотоковому середовищі
////    плюси: ще один пункт це ресайз, но так як буде тільки один масив(кромі нього нічого більше не 
////           треба буде ресайзити), то ресайз буде доволі бистрим в порівнянні з іншими варіантами,
////    
//// вибраний варінт - 3: 
////          цей варіант є найкращим із за простоти в реалізації, швидкому доступу до елементів і бистрому ресайзі,
////          звісно тут йде звернення до центрального компонента, но думаю що це допустимо, просто менеджер треба
////          зробити глобальним
//
//
//class HazardPointerManager {
//  struct FreeListArray {
//    tracked_vector<std::atomic<uint32_t>> _freeList;
//    tracked_vector<std::atomic<void*>> _pointerArray;
//    std::atomic<uint32_t> _capacity;
//    std::atomic<uint32_t> _size;
//    std::atomic<uint32_t> _sizeFreeList;
//    FreeListArray();
//  };
//
//  struct Synchronization {
//    std::atomic<uint32_t> _writerReaderCounter;
//    std::shared_mutex _mutex;
//  };
//
//public:
//  uint32_t* registerThread();
//  void unregisterThread();
//  void retirePointer(void* ptr);
//  void collectRetiredPointers();
//
//private:
//  bool isPointerHazard(void* ptr);
//  void resizeRetiredPointerList(uint32_t expectedSize, uint32_t desiredSize);
//  void resizeHazardPointers(uint32_t expectedSize, uint32_t desiredSize);
//
//private:
//
//  tracked_unordered_map<uint32_t, std::atomic<uint32_t>> m_threadIDToIndexMap;
//  FreeListArray m_hazardPointers;
//  Synchronization m_hazardPointersSync;
//  ///
//  FreeListArray m_retired;
//  Synchronization m_retiredPointersSync;
//
//
//  std::atomic<bool> m_isCollecting;
//};
//
//
//extern HazardPointerManager GHManager;
