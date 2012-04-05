#ifndef TRACE_H_INCLUDED__
#define TRACE_H_INCLUDED__

#define TRACE_ENTERING_FUNCTION__ "Entering"
#define TRACE_LEAVING_FUNCTION__ "Uncaught exception in"
#define TRACE_EXITING_FUNCTION__ "Exiting"
#define TRACE_ENTERING_FUNCTION_W__ L"Entering"
#define TRACE_LEAVING_FUNCTION_W__ L"Uncaught exception in"
#define TRACE_EXITING_FUNCTION_W__ L"Exiting"

#include <string>
#include <sstream>
#include "NonCopyable.h"
#include "Log.h"

namespace Logging
{
	class Trace : protected NonCopyable
	{
	public:
		~Trace()
		{
			if (std::uncaught_exception())
			{
				Log::Get()->Exception(CreateMessage(m_Location, TRACE_LEAVING_FUNCTION__, m_Function));
			}  
			else
			{
				Log::Get()->Trace(CreateMessage(m_Location, TRACE_EXITING_FUNCTION__, m_Function));
			}
		}
		Trace(const char* pszFile, unsigned int line, const char* pszFunc) : m_Location(CreateLocation(pszFile, line)), m_Function(pszFunc)
		{
			Log::Get()->Trace(CreateMessage(m_Location, TRACE_ENTERING_FUNCTION__, m_Function));
		}

	private:
		static std::string CreateLocation(const char* pszFile, unsigned int line)			
		{
			std::ostringstream ss;
			ss << std::string(pszFile) << " (" << line << ")";
			return ss.str();
		}
		static std::string CreateMessage(const std::string& location, const char* pszStatus, const std::string& function)
		{
			std::ostringstream ss;
			ss << location << " " << pszStatus << " " << function << "()";
			return ss.str();
		}

		std::string m_Location;
		std::string m_Function;
	};
}

#endif
