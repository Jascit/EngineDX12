#pragma once
#include <Include/Engine/Core/Interfaces/Multithreading/Mutex/BaseMutex.h>

class Mutex : public BaseMutex<Mutex>
{
public:
	using BaseMutex = BaseMutex<Mutex>;

	Mutex() : BaseMutex() {};
	~Mutex() override = default;
	void _lockImpl() {
		AcquireSRWLockExclusive(&m_lock);
	};
	void _unlockImpl() {
		ReleaseSRWLockExclusive(&m_lock);
	}
};