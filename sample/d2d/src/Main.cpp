/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include <cx/runtime/runtime.hpp>
#include <cx/wui/wui.hpp>

//===========================================================================
#include "Application.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#if 1
#if defined(_DEBUG)
#pragma comment(lib, "../../library/lib/cx_static_library/x64/Debug/cx_static_library.lib")
#else
#pragma comment(lib, "../../library/lib/cx_static_library/x64/Release/cx_static_library.lib")
#endif
#endif
#if 0
#pragma comment(lib, "cx_static_library.lib")
#endif





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
int APIENTRY wWinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPWSTR    lpCmdLine,
	int       nCmdShow)
{
	cx::runtime::memory_debug _memory_debug;
	_memory_debug.enable();
	_memory_debug.start_leak_check();


	cx::wui::getAppModule()->setInstanceHandle(hInstance);
	getApplication()->launch();


	//_memory_debug.test_leak_check();
	_memory_debug.end_leak_check();	
	return 0;
}
