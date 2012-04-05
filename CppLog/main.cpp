#include <memory>
#include "Trace.h"

using namespace std;
using namespace Logging;

void Throws(unsigned i)
{
	Trace __tracer(__FILE__, __LINE__, __FUNCTION__);
	if (i > 0)
	{
		Throws(--i);
	}
	throw std::runtime_error("Fehler!");
}

int main()
{
	// Either read the configuration from the registry or configure the logging subsystem dynamically.
#if 1
	//Log::m_Configuration.reset(new ConfigurationRegistry());
	wstring k(L"Software\\CppLog");
	string f("DebugFile");
	string l("DebugLevel");
	Log::m_Configuration.reset(new ConfigurationRegistry(k, f, l));
#else
	Log::m_Configuration.reset(new Configuration("./cpplog.txt", Logging::LevelDebug));
#endif

	Trace __tracer(__FILE__, __LINE__, __FUNCTION__);
	Log::Get()->Info("*** Starting log session ***");
	{
		Log::Get()->Trace("This is a trace message");
		Log::Get()->Error("This is an error message");
		Log::Get()->Warning("This is a warning message");
		Log::Get()->Exception("This is an exception message");
		Log::Get()->Fatal("This is a fatal message");
		Log::Get()->Info("This is an info message");
		Log::Get()->Debug("This is a debug message");
	}
	{
		Log::Get("Foobar")->Trace("This is a trace message");
		Log::Get("Foobar")->Error("This is an error message");
		Log::Get("Foobar")->Warning("This is a warning message");
		Log::Get("Foobar")->Exception("This is an exception message");
		Log::Get("Foobar")->Fatal("This is a fatal message");
		Log::Get("Foobar")->Info("This is an info message");
		Log::Get("Foobar")->Debug("This is a debug message");
	}
	{
		Log::Get("Baz")->Trace("This is a trace message");
		Log::Get("Baz")->Error("This is an error message");
		Log::Get("Baz")->Warning("This is a warning message");
		Log::Get("Baz")->Exception("This is an exception message");
		Log::Get("Baz")->Fatal("This is a fatal message");
		Log::Get("Baz")->Info("This is an info message");
		Log::Get("Baz")->Debug("This is a debug message");
	}
	{
		Log::Get("Cytrus")->Trace("This is a trace message");
		Log::Get("Cytrus")->Error("This is an error message");
		Log::Get("Cytrus")->Warning("This is a warning message");
		Log::Get("Cytrus")->Exception("This is an exception message");
		Log::Get("Cytrus")->Fatal("This is a fatal message");
		Log::Get("Cytrus")->Info("This is an info message");
		Log::Get("Cytrus")->Debug("This is a debug message");
	}
	{
		Log::Get("Baz")->Trace("This is a trace message");
		Log::Get("Baz")->Error("This is an error message");
		Log::Get("Baz")->Warning("This is a warning message");
		Log::Get("Baz")->Exception("This is an exception message");
		Log::Get("Baz")->Fatal("This is a fatal message");
		Log::Get("Baz")->Info("This is an info message");
		Log::Get("Baz")->Debug("This is a debug message");
	}
	{
		Log::Get("Foobar")->Trace("This is a trace message");
		Log::Get("Foobar")->Error("This is an error message");
		Log::Get("Foobar")->Warning("This is a warning message");
		Log::Get("Foobar")->Exception("This is an exception message");
		Log::Get("Foobar")->Fatal("This is a fatal message");
		Log::Get("Foobar")->Info("This is an info message");
		Log::Get("Foobar")->Debug("This is a debug message");
	}
	{
		try
		{
			Throws(10);
		}
		catch (const std::exception& ex)
		{
			Log::Get()->Exception("Throws() has thrown");
			Log::Get()->Error(ex.what());
		}
	}
	
	Log::Get()->Info("*** Finishing log session ***");
	return 0;
}