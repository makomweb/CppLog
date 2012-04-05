#ifndef SINGLELOCK_H_3DA18A41_BB62_4C84_82EA_8EA9A8B0B609_INCLUDED__
#define SINGLELOCK_H_3DA18A41_BB62_4C84_82EA_8EA9A8B0B609_INCLUDED__

#include <Windows.h>
#pragma comment(lib, "Kernel32.lib")
#include "NonCopyable.h"

/// @brief Class which implements the scoped lock for CRITICAL_SECTION instances.
///
/// This class is needed for programming exception safe.
class ScopedLock : protected NonCopyable
{
public:
	/// Dtor
	/// is responsible for calling leave critical section in case an instance of this
	/// class goes out of scope
	~ScopedLock()
	{
		LeaveCriticalSection(&m_CriticalSection);
	}
	/// Ctor
	/// @param cs [in] reference to the CRITICAL_SECTION instance an instance of this class is working on
	explicit ScopedLock(CRITICAL_SECTION& cs) : m_CriticalSection(cs)
	{
		EnterCriticalSection(&m_CriticalSection);
	}
private:

	CRITICAL_SECTION& m_CriticalSection; ///< reference to the CRITICAL_SECTION an instance of this class is working on
};

#endif
