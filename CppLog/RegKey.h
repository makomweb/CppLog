#ifndef REG_KEY_H_INCLUDED__
#define REG_KEY_H_INCLUDED__

#include <Windows.h>
#pragma comment(lib, "Advapi32.lib")
#include "Win32Exception.h"
#include <vector>
#include <string>

class RegKey
{
public:
	~RegKey() { RegCloseKey(m_Handle); }
	RegKey(HKEY base, const std::wstring& nam, REGSAM sam = KEY_READ | KEY_WOW64_64KEY) : m_Handle(OpenKey(base, nam, sam)) {}
	unsigned long ValueAsUnsignedLong(const std::string& sub)
	{
		unsigned long type = REG_DWORD;
		unsigned long cb = 1024;
		unsigned long val;
		unsigned long res = RegQueryValueEx(m_Handle, sub.c_str(), NULL, &type, (LPBYTE)&val, &cb);
		if (ERROR_SUCCESS != res)
		{
			throw Win32Exception("RegOpenKeyEx() has failed", res, __FILE__, __LINE__);
		}
		return val;
	}
	std::string ValueAsString(const std::string& sub)
	{
		unsigned long type = REG_DWORD;
		unsigned long cb = 1024;
		std::vector<char> buf(cb);
		unsigned long res = RegQueryValueEx(m_Handle, sub.c_str(), NULL, &type, (LPBYTE)&buf[0], &cb);
		if ((ERROR_SUCCESS != res) && (ERROR_INSUFFICIENT_BUFFER != res))
		{
			res = RegQueryValueEx(m_Handle, sub.c_str(), NULL, &type, (LPBYTE)&buf[0], &cb);	
		}
		if (ERROR_SUCCESS != res)
		{
			throw Win32Exception("RegQueryValueEx() has failed", res, __FILE__, __LINE__);
		}
		return std::string(buf.cbegin(), buf.cend());
	}

private:
	static HKEY OpenKey(HKEY base, const std::wstring& nam, REGSAM sam)
	{
		HKEY h;
		DWORD res = RegOpenKeyExW(base, nam.c_str(), 0, sam, &h);
		if (ERROR_SUCCESS != res)
		{
			throw Win32Exception("RegOpenKeyEx() has failed", res, __FILE__, __LINE__);
		}
		return h;
	}
	HKEY m_Handle;
};

#endif
