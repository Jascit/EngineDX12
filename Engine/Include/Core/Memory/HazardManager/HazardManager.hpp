//#pragma once
//#include <Core/Memory/HazardManager/HazardPointer.hpp>
//#include <Utils/WinInclude.hpp>
//#include <shared_mutex>
//#include <thread>
//
//// 3. ����� � ��������, ������ ����������� �� ������������ ���������� ��� ��������� � ��� �������, 
////    ������� ����� ���� ����(+)
////    ����: ������� ������ � ���������, ������� �� ���� ��������� ��� ����������� �������� �������, 
////          ��� � ����������� ������� �� �������� � ����� �� ��������� ��� ���������� ������ 
////    �������: ��� �� �������� �� ������� ����������, �� � �������� � ������ ��������� �� ����������
////              ������� ��������� ����� ��� ������������ �� �������� � ���������������� ����������
////    �����: �� ���� ����� �� ������, �� ��� �� ���� ����� ���� �����(���� ����� ����� ����� �� 
////           ����� ���� ���������), �� ������ ���� ����� ������� � �������� � ������ ���������,
////    
//// �������� ����� - 3: 
////          ��� ������ � ��������� �� �� �������� � ���������, �������� ������� �� �������� � �������� ������,
////          ����� ��� ��� ��������� �� ������������ ����������, �� ����� �� �� ���������, ������ �������� �����
////          ������� ����������
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
