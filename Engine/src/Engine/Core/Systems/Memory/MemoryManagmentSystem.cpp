#include <Include/Engine/Core/Systems/Memory/MemoryManagmentSystem.h>

bool MemoryManagmentSystem::initialize(){
  ErrorHandler::Get().Assert(GMalloc != nullptr, "GMalloc wasn't initialized before MemoryManagmentSystem");
  return true;
}

void MemoryManagmentSystem::shutdown(){
  if (GMalloc)
  {
    Logger::Get().logInfo("Live bytes: " + std::to_string(GMalloc->getTotalAllocated()));
  }
}

void MemoryManagmentSystem::update(){

}

