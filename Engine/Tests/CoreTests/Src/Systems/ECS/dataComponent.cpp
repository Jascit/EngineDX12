#include <Systems/ECS/dataComponent.h>

dataComponent::~dataComponent()  {};
dataComponent::dataComponent(void* ptr) : _ptr(ptr) {};
dataComponent::dataComponent() : _ptr(nullptr) {};
