#ifndef LOG_H_INCLUDED__
#define LOG_H_INCLUDED__

#include <string>
#include <fstream>
#include <memory>
#include <map>
#include <iomanip>
#include <Windows.h>
#pragma comment(lib, "Kernel32.lib")
#include "NonInstanciateable.h"
#include "NonCopyable.h"
#include "ScopedLock.h"
#include "RegKey.h"

namespace Logging
{
	enum Level { LevelOff = 0, LevelFatal, LevelError, LevelException, LevelWarning, LevelInfo, LevelTrace, LevelDebug };

	std::string AsString(SYSTEMTIME st)
	{
		std::stringstream ss;
		ss << std::setfill('0');
		ss << std::setw(2) << st.wMonth;
		ss << "/" << std::setw(2) << st.wDay;
		ss << "/" << std::setw(4) << st.wYear;
		ss << " " << std::setw(2) << st.wHour;
		ss << ":" << std::setw(2) << st.wMinute;
		ss << "." << std::setw(2) << st.wSecond;
		ss << "," << std::setw(3) << st.wMilliseconds;
		return ss.str();
	}

	std::string Now()
	{
		SYSTEMTIME st;
		GetSystemTime(&st);
		return AsString(st);
	}

	unsigned CurrentThreadId()
	{
		return GetCurrentThreadId();
	}

	unsigned CurrentProcessId()
	{
		return GetCurrentProcessId();
	}

	std::string AsString(Level level)
	{
		switch (level)
		{
			case LevelDebug: return std::string("DEBUG"); break;
			case LevelError: return std::string("ERROR"); break;
			case LevelWarning: return std::string("WARN"); break;
			case LevelException: return std::string("EXCPT"); break;
			case LevelFatal: return std::string("FATAL"); break;
			case LevelInfo: return std::string("INFO"); break;
			case LevelTrace: return std::string("TRACE"); break;
			default: return std::string(""); break;
		}
	}

	class ILogger : protected NonCopyable
	{
	public:
		virtual ~ILogger() {}
		virtual void Debug(const std::string& msg) = 0;
		virtual void Trace(const std::string& msg) = 0;
		virtual void Info(const std::string& msg) = 0;
		virtual void Warning(const std::string& msg) = 0;
		virtual void Error(const std::string& msg) = 0;
		virtual void Fatal(const std::string& msg) = 0;
		virtual void Exception(const std::string& msg) = 0;
	};

	class StreamLogger : public ILogger
	{
	public:
		virtual ~StreamLogger() {}
		StreamLogger(const std::string& nam, std::ostream& out, unsigned pid, Level level) :
			m_Name(nam), m_Out(out), m_Pid(pid), m_Level(level)
		{
			InitializeCriticalSection(&m_Sync);
		}
		void Debug(const std::string& msg)
		{
#ifdef _DEBUG
			if (m_Level >= Level::LevelDebug) Write(msg, Level::LevelDebug);
#endif
		}
		void Trace(const std::string& msg)
		{			
#ifdef _DEBUG
			if (m_Level >= Level::LevelTrace) Write(msg, Level::LevelTrace);
#endif
		}
		void Info(const std::string& msg) { if (m_Level >= Level::LevelInfo) Write(msg, Level::LevelInfo); }
		void Warning(const std::string& msg) { if (m_Level >= Level::LevelWarning) Write(msg, Level::LevelWarning); }
		void Error(const std::string& msg) { if (m_Level >= Level::LevelError) Write(msg, Level::LevelError); }
		void Exception(const std::string& msg) { if (m_Level >= Level::LevelException) Write(msg, Level::LevelException); }
		void Fatal(const std::string& msg) { if (m_Level >= Level::LevelFatal) Write(msg, Level::LevelFatal); }

	private:
		void Write(const std::string& msg, Level level)
		{
			ScopedLock lock(m_Sync);
			m_Out << Now() << " " << m_Pid << "/" << CurrentThreadId() << " " << AsString(level) << "\t" << m_Name << " " << msg << std::endl;
		}
		std::string m_Name;
		std::ostream& m_Out;
		unsigned m_Pid;
		Level m_Level;
		CRITICAL_SECTION m_Sync;
	};

	class FileStream : public std::ofstream
	{
	public:
		virtual ~FileStream() { close(); }
		explicit FileStream(const std::string& path) : std::ofstream(path, std::ios_base::ate) {}
	};

	class NirvanaStream : public std::ostream
	{
	public:
		virtual ~NirvanaStream() {}
		NirvanaStream() : std::ostream(0) {}
	};
	
	class Configuration
	{
	public:
		Configuration() : m_LogFilePath(CreateLogFilePath()), m_LogLevel(CreateLogLevel()), m_ProcessId(CurrentProcessId()) {}
		Configuration(const std::string& path, Level level) : m_LogFilePath(path), m_LogLevel(level), m_ProcessId(CurrentProcessId()) {}
		Configuration(const std::string& path, Level level, unsigned pid) : m_LogFilePath(path), m_LogLevel(level), m_ProcessId(pid) {}
		std::shared_ptr<ILogger> CreateLogger(std::ostream& os) const
		{
			return CreateLogger("*", os);
		}
		std::shared_ptr<ILogger> CreateLogger(const std::string& nam, std::ostream& os) const
		{
			return std::shared_ptr<ILogger>(new StreamLogger(nam, os, m_ProcessId, m_LogLevel));
		}
		std::ostream* CreateOutput() const
		{
			if ((m_LogFilePath.length() == 0) || (m_LogLevel == LevelOff))
			{
				return new NirvanaStream();
			}			
			return new FileStream(m_LogFilePath);
		}

	private:
		static std::string CreateLogFilePath()
		{
			SYSTEMTIME st;
			GetSystemTime(&st);
			std::stringstream ss;
			ss << st.wMonth << " " << st.wDay << " " << st.wYear << " application.log";
			return ss.str();
		}
		static Level CreateLogLevel()
		{
			return LevelDebug;
		}

	protected:
		std::string m_LogFilePath;
		Level m_LogLevel;
		unsigned m_ProcessId;
	};

	class ConfigurationRegistry : public Configuration
	{
	public:
		ConfigurationRegistry() :
			Configuration(CreateLogFilePath(std::wstring(L"Software\\CppLog"), std::string("DebugFile")),
						  CreateLogLevel(std::wstring(L"Software\\CppLog"), std::string("DebugLevel")),
						  CurrentProcessId())
		{}

		ConfigurationRegistry(const std::wstring& key, const std::string& propFile, const std::string& propLevel) :
			Configuration(CreateLogFilePath(key, propFile),
						  CreateLogLevel(key, propLevel),
						  CurrentProcessId())
		{}

	private:
		static std::string CreateLogFilePath(const std::wstring& key, const std::string& propFile)
		{
			try
			{
				RegKey obj(HKEY_LOCAL_MACHINE, key, KEY_READ | KEY_WOW64_64KEY);
				return obj.ValueAsString(propFile);
			}
			catch (...)
			{
			}
			return std::string("");
		}
		static Level CreateLogLevel(const std::wstring& key, const std::string& propLevel)
		{
			try
			{
				RegKey obj(HKEY_LOCAL_MACHINE, key, KEY_READ | KEY_WOW64_64KEY);
				return (Level)obj.ValueAsUnsignedLong(propLevel);
			}
			catch (...)
			{
			}
			return Level::LevelOff;
		}
	};

	template <class ConfigurationPolicy = Configuration>
	class LogHost : protected NonInstanciateable
	{
	public:
		static std::shared_ptr<ILogger> Get()
		{
			return Get(std::string("*"));
		}
		static std::shared_ptr<ILogger> Get(const std::string& nam)
		{
			std::map<std::string, std::shared_ptr<ILogger> >::const_iterator it = m_LoggerMap.find(nam);
			if (it != m_LoggerMap.cend())
			{
				return it->second;
			}
			if (!m_Configuration)
			{
				m_Configuration.reset(new ConfigurationPolicy());
			}
			if (!m_Output)
			{
				m_Output.reset(m_Configuration->CreateOutput());
			}
			std::shared_ptr<ILogger> logger(m_Configuration->CreateLogger(nam, *m_Output));
			m_LoggerMap.insert(std::pair<std::string, std::shared_ptr<ILogger> >(nam, logger));
			return logger;
		}

		static std::unique_ptr<ConfigurationPolicy> m_Configuration;

	private:
		static std::unique_ptr<std::ostream> m_Output;
		static std::map<std::string, std::shared_ptr<ILogger> > m_LoggerMap;
	};

	template <class ConfigurationPolicy>
	std::unique_ptr<ConfigurationPolicy> LogHost<ConfigurationPolicy>::m_Configuration;

	template <class ConfigurationPolicy>
	std::unique_ptr<std::ostream> LogHost<ConfigurationPolicy>::m_Output;

	template <class ConfigurationPolicy>
	std::map<std::string, std::shared_ptr<ILogger> > LogHost<ConfigurationPolicy>::m_LoggerMap;

	typedef LogHost<Configuration> Log;
}

#endif
