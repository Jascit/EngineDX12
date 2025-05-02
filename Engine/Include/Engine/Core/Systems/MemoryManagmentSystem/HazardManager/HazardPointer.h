#pragma once

class HazardPointer {
public:
	HazardPointer(const HazardPointer&) = delete;
	HazardPointer operator=(const HazardPointer&) = delete;

	HazardPointer();
	HazardPointer(void* ptr);
	
	void* Get() {
		return ptr;
	}
	void Set(void* ptr) {
		m_ptr = ptr;
	}

	HazardPointer& operator=(const HazardPointer& other);
	HazardPointer& operator!=(const HazardPointer& other);
	HazardPointer& operator!=(const void* ptr);
	HazardPointer& operator==(const HazardPointer& other);
	HazardPointer& operator==(const void* ptr);
	
private:
	void* m_ptr;
};
