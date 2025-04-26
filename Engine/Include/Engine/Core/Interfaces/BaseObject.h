#pragma once
#include <iostream>
#include <string>
#include <typeinfo>

class BaseObject {
protected:
  std::string name;         // Ім'я об'єкта
  uint32_t instanceNumber;       // Унікальний номер екземпляру

  // Статичний лічильник для відслідковування кількості створених об'єктів
  static std::atomic<uint32_t> instanceCount;

public:
  // Конструктор
  BaseObject() {
    instanceCount++;
    instanceNumber = instanceCount.load();
    name = "Default_" + std::string(typeid(*this).name()) + "_" + std::to_string(instanceNumber);
  }

  virtual ~BaseObject() = default;
  std::string const& GetName() {
    return name;
  }
  void SetName(const std::string& newName) {
    name = newName;
  }
  int GetInstanceNumber() const {
    return instanceNumber;
  }
  static int GetInstanceCount();
};