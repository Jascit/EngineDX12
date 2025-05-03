#pragma once
#include <thread>

class HazardPointer {
public:
	HazardPointer();
	HazardPointer(void* ptr);
	HazardPointer(const HazardPointer&) = delete;
	
	void* Get() {
		return ptr;
	}
	void Set(void* ptr) {
		m_ptr = ptr;
	}


	HazardPointer operator=(const HazardPointer&) = delete;
	HazardPointer& operator!=(const HazardPointer& other);
	HazardPointer& operator!=(const void* ptr);
	HazardPointer& operator==(const HazardPointer& other);
	HazardPointer& operator==(const void* ptr);
	
private:
	void* m_ptr;
	std::thread
};
