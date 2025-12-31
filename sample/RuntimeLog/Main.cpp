//////////////////////////////////////////////////////////////////////////////
//============================================================================
#include "pch.hpp"

//============================================================================
#include <cx/runtime/log_facility/log_facility.hpp>





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#if defined(_DEBUG)
#pragma comment(lib, "../../library/lib/cx_static_library/x64/Debug/cx_static_library.lib")
#else
#pragma comment(lib, "../../library/lib/cx_static_library/x64/Release/cx_static_library.lib")
#endif


//////////////////////////////////////////////////////////////////////////////
//============================================================================
#if 0
C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Tools\MSVC\14.44.35207\include
C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Tools\MSVC\14.44.35207\atlmfc\include
C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\VS\include
C:\Program Files (x86)\Windows Kits\10\Include\10.0.26100.0\ucrt
C:\Program Files (x86)\Windows Kits\10\Include\10.0.26100.0\um
C:\Program Files (x86)\Windows Kits\10\Include\10.0.26100.0\shared
C:\Program Files (x86)\Windows Kits\10\Include\10.0.26100.0\winrt
C:\Program Files (x86)\Windows Kits\10\Include\10.0.26100.0\cppwinrt
C:\Program Files (x86)\Windows Kits\NETFXSDK\4.8\Include\um
#endif


//////////////////////////////////////////////////////////////////////////////
//============================================================================
void test_cx_runtime_log()
{
	bool rv = cx::runtime::log_facility_initialize();
	if (false == rv)
	{
		printf("Failed to initialize log item memory.\n");
		return;
	}


	CX_RUNTIME_LOG(cxLInfo) << L"start";

	CX_RUNTIME_LOG(cxLTrace) << L"This is a trace log message.";
	CX_RUNTIME_LOG(cxLDebug) << L"This is a debug log message.";
	CX_RUNTIME_LOG(cxLInfo) << L"This is an info log message.";
	CX_RUNTIME_LOG(cxLWarning) << L"This is a warning log message.";
	CX_RUNTIME_LOG(cxLError) << L"This is an error log message.";
	CX_RUNTIME_LOG(cxLCritical) << "This is a critical log message.";

	Sleep(1000); // Simulate some processing time
	CX_RUNTIME_LOG(cxLInfo) << L"end";


	cx::runtime::log_facility_flush();
	cx::runtime::log_facility_cleanup();
}





//////////////////////////////////////////////////////////////////////////////
//============================================================================
int main()
{
	test_cx_runtime_log();

	return 0;
}