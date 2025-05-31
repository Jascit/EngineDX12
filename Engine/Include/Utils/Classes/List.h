#include <Core/Memory/STLAllocator.h>
#include <atomic>
/* 
 * idea:
 * 1->2->3->4->5->dead           /1.
 * 1->2->3->4->5->9->10          /2
 * dead->6->7->8                 /2.
 * 1->2->3->4->5->9->10->6->7->8 /3. 
 */

/*
* new idea:
* <-m_head-sync_point-m_tail->
* pop_front: sync_point-m_tail->, pop_front one more: sync_point
* pop_back: <-m_head-sync_point, one more time: sync_point 
*/

/*
* нова ідея: рекурсивно доставляти ссилку на елемент для останньої 
* голови за допомогою живих читачів, голова добавляється завжди спереді, 
* даже якщо після мертвої
* 
*  
*/

//lock-free, thread safe, based on Nodes
template<typename T>
class List {
  enum ListFlags : uint16_t
  {
    NONE = 0,
    TO_DESTROY
  };
  struct Node {
    T _data;
    std::atomic<uint32_t> _readersCount;
    std::atomic<ListFlags> _flag;
    std::atomic<Node*> _next; // ->
    std::atomic<Node*> _prev; // <-

    Node() : _next(nullptr), _prev(nullptr), _data(), _flag(ListFlags::NONE) {};
    Node(T data) : _next(nullptr), _prev(nullptr), _data(std::forward<T>(data)), _flag(ListFlags::NONE) {};
   };
  struct Node {
    std::atomic<Node*> _next; // ->
    std::atomic<Node*> _prev; // <-

    Node() : _next(nullptr), _prev(nullptr), _data(), _flag(ListFlags::NONE) {};
  };
public:
  void push_back(T&& data) {
    // allocate and construct new node
    Node* instance = m_alloc.allocate(1);
    m_alloc.construct(instance, std::forward<T>(data));

    // if list is empty, try to install as tail and head
    if (m_size.load(std::memory_order_acquire) <= 0) {
      instance->_flag.store(ListFlags::SYNC_POINT, std::memory_order_release);
      Node* expected = nullptr;
      if (m_synchPoint.compare_exchange_strong(expected, instance)) {
        
        m_size.fetch_add(1, std::memory_order_release);
        return;
      }
      instance->_flag.store(ListFlags::NONE, std::memory_order_release);
    }

    // otherwise, find current tail and link new node after it
    Node* currentTail = (m_size.load(std::memory_order_acquire) == 1) ? m_tail.load(std::memory_order_acquire) : m_synchPoint.load(std::memory_order_acquire); //////
    Node* expected = nullptr;
    while (!currentTail->_next.compare_exchange_strong(expected, instance)) {
      currentTail = currentTail->_next.load(std::memory_order_acquire);
      expected = nullptr;
    }
    // set back-pointer from new node to old tail
    instance->_prev.store(currentTail, std::memory_order_release);
    // install new tail
    m_tail.store(instance, std::memory_order_release);
    // increment size
    m_size.fetch_add(1, std::memory_order_release);
  }

  void push_front(T&& data) {
    Node* instance = m_alloc.allocate(1);
    m_alloc.construct(instance, data);
    if (m_size.load(std::memory_order_acquire) <= 0) {
      Node* expected = nullptr;
      if (m_head.compare_exchange_strong(expected, instance))//just to be sure
      {
        m_size.fetch_add(1, std::memory_order_release);
        return;
      }
    }
    Node* currentHead = m_head.load(std::memory_order_acquire);
    Node* expected = nullptr;
    while (!currentHead->_prev.compare_exchange_strong(expected, instance)) {
      currentHead = currentHead->_prev.load(std::memory_order_acquire);
    }
    instance->_next.store(currentHead, std::memory_order_release);
    m_head.store(instance, std::memory_order_release);
    m_size.fetch_add(1, std::memory_order_release);
  }

  const T pop_front_return() {
    int32_t size = m_size.fetch_sub(1, std::memory_order_acq_rel);
    if (size < 0) {   
      m_size.fetch_add(1, std::memory_order_release);
      return T{};
    }
    Node* headToDelete = m_head.load(std::memory_order_acquire);
    headToDelete->_readersCount.fetch_add(1, std::memory_order_release); // we need to safe a node before ours, so it will be not deleted when we needed, so one thread will be looking for 2 Nodes for the same time
    Node* aliveNodes = headToDelete->_prev.exchange(nullptr, std::memory_order_acq_rel);// taking the Node before our dead Node with exchange, so pop_back won't working on this Node and will return 0
    if (size == 0) // if size of ours List was 1
    {
      if (m_synchPoint.compare_exchange_strong(headToDelete, nullptr)) { // delete synch_point or go ahead
        T dataInstance = headToDelete->_data; // make copy
        headToDelete->_readersCount.fetch_sub(1, std::memory_order_release);
        headToDelete->_readersCount.wait(0, std::memory_order_acquire);
        m_alloc.deallocate(headToDelete, 1);// just die mf
        return dataInstance;
      }
    }
    ListFlags expected = ListFlags::NONE;
    Node* temp = nullptr;
    uint8_t counter = 0;
    while (!headToDelete->_flag.compare_exchange_strong(expected, ListFlags::TO_DESTROY))//in case if our m_head is a dead Node and there are dead Nodes before
    {
      counter++;
      if (counter == 3) // we are needing only 2 nodes, another we can release, don't care about it
      {
        temp = headToDelete->_prev.load(std::memory_order_acquire); // 3. Node
        temp->_readersCount.fetch_sub(1, std::memory_order_release); // release 3. Node
        temp->_readersCount.notify_all(); // notify all threads(but in most cases it will be only one), so they can delete the node
      }
      headToDelete = headToDelete->_next.load(std::memory_order_acquire);
      if (headToDelete == nullptr)//error
      {
        return T{};
      }
      headToDelete->_readersCount.fetch_add(1, std::memory_order_release);
    }
    Node* headBefore = headToDelete->_next.load(std::memory_order_acquire);
    m_head.compare_exchange_strong(headToDelete, headBefore);//if somebody has already changed it, it's not the problem, even if it's a dead Node
    headBefore->_prev.compare_exchange_strong(headToDelete, nullptr); // make sure that nobody will go through the dead Node anymore / only one thread at a time can change _prev / headToDeleate | headBefore->...->...
    if (aliveNodes != nullptr)// if there is any Nodes after our deadNode
    {
      //in case he's already dead, just cut and leave it, our job will do another thread. problem: someone can delete Node after ours, before we access it
      if (aliveNodes->_flag.load(std::memory_order_acquire) == ListFlags::TO_DESTROY)
      {
        aliveNodes->_prev.load(std::memory_order_acquire)->_next.store(nullptr, std::memory_order_acquire);
        aliveNodes->_readersCount.fetch_sub(1, std::memory_order_release);
        aliveNodes->_readersCount.notify_all();
      }
      else {
        Node* currentHead = headBefore; // in case if someone already has exchanged head
        Node* expected = nullptr;
        while (!currentHead->_prev.compare_exchange_strong(expected, aliveNodes)) {//fighting with other threads for placement for our Node
          if (currentHead->_flag.load(std::memory_order_acquire) == ListFlags::TO_DESTROY)//if this Node is going to be deleted, we just make step back, don't risk it
          {
            Node* expectedNode = currentHead->_next.load(std::memory_order_acquire);
            Node* desired = nullptr;
            expectedNode->_prev.compare_exchange_strong(currentHead, desired);
            currentHead = expectedNode;
          }
          else
          {
            currentHead = currentHead->_prev.load(std::memory_order_acquire);
          }
        }
        aliveNodes->_next.store(currentHead, std::memory_order_release);// that can be a problem
        m_head.store(aliveNodes, std::memory_order_release);
      }
    }
    T dataInstance = headToDelete->_data; // make copy
    headToDelete->_readersCount.fetch_sub(1, std::memory_order_release);
    headToDelete->_readersCount.wait(0, std::memory_order_acquire);
    m_alloc.deallocate(headToDelete, 1);// just die mf
    return dataInstance;
  }

  const T pop_back_return() {
    // 1. Decrement the size
    int32_t size = m_size.fetch_sub(1, std::memory_order_acq_rel);
    if (size < 0) {
      // If the list is empty — roll back and return default
      m_size.fetch_add(1, std::memory_order_release);
      return T{};
    }

    // 2. Load current tail and "lock" it from being deleted
    Node* tailToDelete = m_tail.load(std::memory_order_acquire);
    tailToDelete->_readersCount.fetch_add(1, std::memory_order_release);

    // 3. Detach the next node (live nodes after the tail)
    Node* aliveNodes = tailToDelete->_next.exchange(nullptr, std::memory_order_acq_rel);

    // 4. If there was only one element
    if (size == 0) {
      if (m_tail.compare_exchange_strong(tailToDelete, nullptr)) {
        T dataInstance = tailToDelete->_data;
        tailToDelete->_readersCount.fetch_sub(1, std::memory_order_release);
        tailToDelete->_readersCount.wait(0, std::memory_order_acquire);
        m_alloc.deallocate(tailToDelete, 1);
        return dataInstance;
      }
    }

    // 5. Wait until tail is marked as TO_DESTROY
    ListFlags expected = ListFlags::NONE;
    Node* temp = nullptr;
    uint8_t counter = 0;
    while (!tailToDelete->_flag.compare_exchange_strong(expected, ListFlags::TO_DESTROY)) {
      if (++counter == 3) {
        // Release third node if it's unnecessary
        temp = tailToDelete->_next.load(std::memory_order_acquire);
        temp->_readersCount.fetch_sub(1, std::memory_order_release);
        temp->_readersCount.notify_all();
      }
      // Move in the reverse direction (towards the head)
      tailToDelete = tailToDelete->_prev.load(std::memory_order_acquire);
      if (tailToDelete == nullptr)//error
      {
        return T{};
      }
      tailToDelete->_readersCount.fetch_add(1, std::memory_order_release);
    }

    // 6. Replace the tail, unlink the deleted node
    Node* tailBefore = tailToDelete->_prev.load(std::memory_order_acquire);
    m_tail.compare_exchange_strong(tailToDelete, tailBefore);
    tailBefore->_next.compare_exchange_strong(tailToDelete, nullptr);

    // 7. If there are any nodes after the deleted one
    if (aliveNodes) {
      if (aliveNodes->_flag.load(std::memory_order_acquire) == ListFlags::TO_DESTROY) {
        // If it's already marked — cut it out
        aliveNodes->_next.load(std::memory_order_acquire)->_prev.store(nullptr, std::memory_order_acquire);
        aliveNodes->_readersCount.fetch_sub(1, std::memory_order_release);
        aliveNodes->_readersCount.notify_all();
      }
      else {
        // Compete for a place at the tail
        Node* currentTail = tailBefore;
        Node* expectedNull = nullptr;
        while (!currentTail->_next.compare_exchange_strong(expectedNull, aliveNodes)) {
          if (currentTail->_flag.load(std::memory_order_acquire) == ListFlags::TO_DESTROY) {
            Node* nextNode = currentTail->_prev.load(std::memory_order_acquire);
            nextNode->_next.compare_exchange_strong(currentTail, nullptr);
            currentTail = nextNode;
          }
          else {
            currentTail = currentTail->_next.load(std::memory_order_acquire);
          }
          expectedNull = nullptr;
        }
        aliveNodes->_prev.store(currentTail, std::memory_order_release);
        m_tail.store(aliveNodes, std::memory_order_release);
      }
    }

    // 8. Copy data and deallocate the node
    T dataInstance = tailToDelete->_data;
    tailToDelete->_readersCount.fetch_sub(1, std::memory_order_release);
    tailToDelete->_readersCount.wait(0, std::memory_order_acquire);
    m_alloc.deallocate(tailToDelete);
    return dataInstance;
  }

  bool empty() noexcept {
    return m_size.load(std::memory_order_acquire) ? false : true;
  }

  inline uint32_t size() noexcept {
    return m_size.load(std::memory_order_acquire);
  }

private:
  Node* pop() {
    int32_t size = m_size.fetch_sub(1, std::memory_order_acq_rel);
    if (size < 0) {
      m_size.fetch_add(1, std::memory_order_release);
    }else if (size == 0) {

    }

    if (m_headSize.load(std::memory_order_acquire) >= m_tailSize.load(std::memory_order_acquire))
    {

    }
  };
private:
  STLAllocator<Node> m_alloc;
  std::atomic<int32_t> m_size;
  std::atomic<Node*> m_head;
  std::atomic<uint32_t> m_headSize;
  std::atomic<Node*> m_synchPoint;
  std::atomic<Node*> m_tail;
  std::atomic<uint32_t> m_tailSize;
};
