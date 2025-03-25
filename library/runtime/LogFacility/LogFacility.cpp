/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include "../runtime.hpp"
#include "LogItemMemory.hpp"
#include "LogItemQueue.hpp"
#include "LogWriter.hpp"
#include "LogAsyncWriter.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::runtime
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static LogWriter* _LogWriter = nullptr;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
bool LogFacility_Initialize(void)
{
	bool rv;

	rv = LogItemMemory_Initialize();
	if (!rv)
	{
		return false;
	}

#if 1
	_LogWriter = cpp_new LogAsyncWriter();
#else
	_LogWriter = cpp_new LogWriter();
#endif

	getLogger()->setOutputHandler(std::bind(&LogWriter::write, _LogWriter, std::placeholders::_1));

	return true;
}

void LogFacility_Cleanup(void)
{
	if (_LogWriter)
	{
		cpp_delete _LogWriter;
		_LogWriter = nullptr;
	}

	LogItemMemory_Cleanup();
}

void LogFacility_Flush(void)
{
	if (_LogWriter)
	{
		_LogWriter->flush();
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




